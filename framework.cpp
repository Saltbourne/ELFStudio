#include "framework.h"
#include "ELFStudio_autogen/include/ui_framework.h"
#include "Binary.h"
#include "Symbol.h"
#include "Section.h"
#include "gelf.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <bfd.h>

/*Initialize GUI and call database connection.
 * - Rebecca L*/
Framework::Framework(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Framework)
{
    QSqlDatabase db;
    this->DBConnect();
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0); //sets tab to start on when app opens.
    ui->stringTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //resize table elements to fit screen.
    ui->sectTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->impTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->libTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QObject::connect(ui->openButton, &QAction::triggered, this, &Framework::FileOpen);
}

/*Destructor.
 * - Rebecca L*/
Framework::~Framework()
{
    delete ui;
    db.close();
}

/*Connect to sqlite database.
 * - Rebecca L*/
void Framework::DBConnect(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("sus_pats.db");
    bool ok = db.open();
    if(ok){
        qDebug() << "The DB is successfully loaded.";
    }
    else{
        qDebug() << "DB failed to open.";
        err.showMessage("Database failed to load.");
    }
}
/*Any ELF file can be opened and read from the file explorer.
 *Reads in header, sections, symbols, and strings info needed for GUI display.
 * - Rebecca L*/
void Framework::FileOpen(){
    QFileDialog dialog(this); // open native file explorer.
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fName = dialog.getOpenFileName();
    dialog.close();

    QFile f(fName);
    if (!f.open(QIODevice::ReadOnly)){
        qDebug() << "NO";
        err.showMessage("The file cannot be opened due to permissions access or it is not an ELF file.");
        return;
    }
    qDebug() << "you just opened up";
    QByteArray data = f.readAll(); // read in file data in byte array.
    ui->lineEdit->setText(fName); // set file path name.

    ui->stringTable->clearContents(); // clear table entries every time new ELF file is open.
    ui->stringTable->setSortingEnabled(false);
    ui->stringTable->setRowCount(0);
    ui->sectTable->clearContents();
    ui->sectTable->setColumnCount(0);
    ui->impTable->setSortingEnabled(false);
    ui->impTable->clearContents();
    ui->impTable->setRowCount(0);
    ui->libTable->setSortingEnabled(false);
    ui->libTable->clearContents();
    ui->libTable->setRowCount(0);

    this->FileStrings(data, data.length());
    this->FileHeader(data[4]); // 4th byte in ELF header gives addressable space info.
    float e = this->FileEntropy(data, data.length());
    ui->lineEdit_2->setText(QString::number(data.length()) + " bytes");
    ui->lineEdit_7->setText(QString::number(e, 'g', 4));

    QString h1 = this->CalculateHash(data, data.length(), EVP_md5(), MD5_DIGEST_LENGTH);
    QString h2 = this->CalculateHash(data, data.length(), EVP_sha1(), SHA_DIGEST_LENGTH);
    QString h3 = this->CalculateHash(data, data.length(), EVP_sha256(), SHA256_DIGEST_LENGTH);
    ui->lineEdit_4->setText(h1);
    ui->lineEdit_5->setText(h2);
    ui->lineEdit_6->setText(h3);

    this->Load_Symbols(fName.toStdString());
    this->Load_Libraries(fName.toStdString());
    f.close();
}

/*Is the ELF file 32-bit or 64-bit based on the file header?
 * - Rebecca L*/
void Framework::FileHeader(int b){
    qDebug() << b;
    if(b == 1){
        ui->lineEdit_3->setText("32 Bit");
    }

    else {
        ui->lineEdit_3->setText("64 Bit");
    }
}

/*Entropy  of file. Template to accept QByteArray or char* data types.
 - Rebecca L*/
template <typename T>
float Framework::FileEntropy(T d, int sz){
    std::map<int,int> freq;

    for(int i = 0; i < sz; i++){
        int count = (int)(d[i]);
        freq[count]++;
    }
    float entropy = 0;
    for(auto i = freq.begin(); i != freq.end(); i++){ // Claude Shannon's formula.
        float p = (float)i->second / (float)sz;
        if (p > 0){
              entropy += p * log2(p);
         }
    }
    entropy *= -1;
    qDebug() << entropy;
    return entropy;

}

/*Calculates hashes using OpenSSL.
 - Rebecca L*/
template <typename T>
QString Framework::CalculateHash(T d, int sz, const EVP_MD* evp, int len){
    unsigned char buf [EVP_MAX_MD_SIZE]= {0};
    unsigned int buf_sz;
    EVP_Digest (d, sz, buf, &buf_sz, evp, NULL);
    char buf2 [EVP_MAX_MD_SIZE];
    QString hash = "";
    for(int i = 0; i < buf_sz; i++){
        sprintf(buf2, "%02x", buf[i]);
        hash.append(buf2);
    }
    return hash;
}

/* Based on FreeBSD strings program in C,
 * reads in byte array and grabs strings 4 characters or longer.
 * Distinguishes between Unicode and Ascii based on char value.
 - Rebecca L*/
void Framework::FileStrings(QByteArray d, int sz){
    std::string buf;
    uint8_t c;
    bool unicode = false;

    for(int i = 0; i < sz; i++) {
      c = d[i];
      if ((('\0' == c || '\n' == c) && buf.length() >= 4)
          || buf.length() == 512) {
          ui->stringTable->insertRow(ui->stringTable->rowCount());
          if(unicode == true){
              ui->stringTable->setItem(ui->stringTable->rowCount()-1, 0, new QTableWidgetItem("Unicode"));
              unicode = false;
          }
          else{
              ui->stringTable->setItem(ui->stringTable->rowCount()-1, 0, new QTableWidgetItem("Ascii"));
          }
          ui->stringTable->setItem(ui->stringTable->rowCount()-1, 1, new QTableWidgetItem(QString::number(buf.length())));
          ui->stringTable->setItem(ui->stringTable->rowCount()-1, 2, new QTableWidgetItem(QString::fromStdString(buf)));
          buf = "";
      } else if (' ' <= c || '\t' == c){
          if(c > 177){
              unicode = true;
          }
          buf.push_back(char(c));
      }
      else{
          buf = "";
      }
    }
    ui->stringTable->setSortingEnabled(true);
}

void Framework::Load_Sections(Binary* b){
    Section *section = new Section(b);
    int ret_sections = section -> load_section();
    std::cout << "The return value of Load section is: " << ret_sections << std::endl;

    std::string name;
    uint64_t size;
    uint64_t r_size;
    uint64_t v_addr;
    uint64_t l_addr;
    uint8_t *bytes;
    for(int i = 0; i < b->sections.size(); i++){
        name = b->sections[i].get_section_name();
        if(name == ".text" || name == ".data" || name == ".symtab" || name == ".dynsym" || name == ".dynamic"){

            size = b->sections[i].get_section_size();
            r_size = b->sections[i].get_section_rsize();
            v_addr = b->sections[i].get_virt_address();
            l_addr = b->sections[i].get_load_address();
            bytes = b->sections[i].get_bytes();

            ui->sectTable->insertColumn(ui->sectTable->columnCount());
            ui->sectTable->setHorizontalHeaderItem(ui->sectTable->columnCount()-1, new QTableWidgetItem("value"));
            ui->sectTable->setItem(0,ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::fromStdString(name)));
            QString h = this->CalculateHash(bytes, size, EVP_md5(), MD5_DIGEST_LENGTH);
            float e = this->FileEntropy(bytes, size);
            ui->sectTable->setItem(1, ui->sectTable->columnCount()-1, new QTableWidgetItem(h));
            ui->sectTable->setItem(2, ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::number(e)));
            ui->sectTable->setItem(3, ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::number(size)));
            ui->sectTable->setItem(4, ui->sectTable->columnCount()-1, new QTableWidgetItem("0x" + QString::number(v_addr)));
            ui->sectTable->setItem(5, ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::number(r_size)));
            ui->sectTable->setItem(6, ui->sectTable->columnCount()-1, new QTableWidgetItem("0x" + QString::number(l_addr)));
        }
    }
}

void Framework::Load_Symbols(std::string filename){
    Binary *binary = new Binary();

    int ret = 0;
    ret = binary -> open_binary_file(filename);

    std::cout << "The return value for open binary is: " << ret << std::endl;

    while(ret < 0)
    {
        std::cout << "Please choose a different file. " << std::endl;
        err.showMessage("Failed to read symbols. The file is likely stripped or the wrong format.");
        return;
    }

    uint64_t entry = bfd_get_start_address(binary->storebfd);
    printf("C style entry@0x%016jx\n", entry);
    ui->lineEdit_8->setText("0x" + QString::number(entry));

    ret = binary -> load_binary(binary);

    std::cout << "The return value of load binary is: " << ret << std::endl;

    Symbol *symbol = new Symbol(binary);

    int ret_static = symbol -> load_static_symbols();
    int ret_dynamic = symbol -> load_dynamic_symbols();

    std::cout << "The return value of static symbols is: " << ret_static << std::endl;
    std::cout << "The return value of dynaic symbols is: " << ret_dynamic << std::endl;

    int size = binary -> symbols.size();
    std::cout << "The size of the symbol vector is: " << size << std::endl;

    for(int i = 0; i < size; i++)
    {
        std::cout << binary -> symbols[i].sym_type << "   " << binary -> symbols[i].get_demangled_name() << "   " << "0x" << binary->symbols[i].get_demangled_name().length() << std::endl;
        if(binary->symbols[i].get_demangled_name().length() > 0){
            ui->impTable->insertRow(ui->impTable->rowCount());
            ui->impTable->setItem(ui->impTable->rowCount()-1, 0, new QTableWidgetItem(QString::fromStdString(binary->symbols[i].get_demangled_name())));
            QSqlQuery query("SELECT name FROM func");
            while (query.next()) {
                QString f = query.value(0).toString();
                if(f == QString::fromStdString(binary->symbols[i].get_demangled_name())){
                    QTableWidgetItem* flag = new QTableWidgetItem("x");
                    flag->setForeground(QBrush(QColor(255, 0, 0)));
                    ui->impTable->setItem(ui->impTable->rowCount()-1, 1, flag);
                    break;
                }
            }
        }

    }
    ui->impTable->setSortingEnabled(true);
    this->Load_Sections(binary);
}

void Framework::Load_Libraries(std::string filename)
{
    //courtesy of stackoverflow:
    //
    //using if statements instead of asser macro to prevent program from aborting.
    const char *file = filename.c_str();
    if(elf_version(EV_CURRENT) == EV_NONE)
    {
        std::cerr << "File type is not supported" << std::endl;
        err.showMessage("File type is not supported");
        return;
    }

    int fd = open(file, O_RDONLY, 0);
    if(fd < 0)
    {
        std::cerr << "File failed to load" << std::endl;
        err.showMessage("File failed to load");
        return;
    }
    Elf *elf = elf_begin(fd, ELF_C_READ, NULL);

    if(elf == nullptr)
    {
        std::cerr << "ELF Begin failed" << std::endl;
        err.showMessage("ELF begin function failed");
        return;
    }

    if(elf_kind(elf) != ELF_K_ELF)
    {
        std::cerr << "ELF kind failed" << std::endl;
        err.showMessage("ELF kind function failed");
        return;
    }

    Elf_Scn *scan = nullptr;
    while((scan = elf_nextscn(elf, scan)) != nullptr)
    {
        GElf_Shdr shdr = {};  //initialize
        if(gelf_getshdr(scan, &shdr) != &shdr)
        {
            std::cerr << "get shdr failed" << std::endl;
            err.showMessage("getshdr function failed");
            return;
        }

        if(shdr.sh_type == SHT_DYNAMIC)
        {
            Elf_Data *data = nullptr;
            data = elf_getdata(scan, data);
            if(data == nullptr)
            {
                std::cerr << "Data is null" << std::endl;
                err.showMessage("Data is null");
                return;
            }

            size_t sh_entsize = gelf_fsize(elf, ELF_T_DYN, 1, EV_CURRENT);

            for(size_t i = 0; i < shdr.sh_size / sh_entsize; i++)
            {
                GElf_Dyn dyn = {};
                if(gelf_getdyn(data, i, &dyn) != &dyn)
                {
                    std::cerr << "getdyn failed" << std::endl;
                    err.showMessage("getdyn failed.");
                    return;
                }
                if(dyn.d_tag == DT_NEEDED)
                {
                    std::cout << "Library: " << elf_strptr(elf, shdr.sh_link, dyn.d_un.d_val) << std::endl;
                    ui->libTable->insertRow(ui->libTable->rowCount());
                    ui->libTable->setItem(ui->libTable->rowCount()-1, 0, new QTableWidgetItem(QString::fromStdString(elf_strptr(elf, shdr.sh_link, dyn.d_un.d_val))));
                }
            }
            ui->libTable->setSortingEnabled(true);
        }

    }
    if(elf_end(elf) != 0)
    {
        std::cerr << "ELF did not end" << std::endl;
        err.showMessage("ELF did not send.");
        return;
    }
    int close(fd);
}

