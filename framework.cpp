#include "framework.h"
#include "ELFStudio_autogen/include/ui_framework.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <bfd.h>

Framework::Framework(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Framework)
{
    QSqlDatabase db;
    this->DBConnect();
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    ui->stringTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->sectTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QObject::connect(ui->openButton, &QAction::triggered, this, &Framework::FileOpen);
}

Framework::~Framework()
{
    delete ui;
    db.close();
}

void Framework::DBConnect(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("sus_pats.db");
    bool ok = db.open();
    if(ok){
        qDebug() << "The DB is successfully loaded.";
    }
    else{
        qDebug() << "DB failed to open.";
    }
}

void Framework::FileOpen(){
    /*Any ELF file can be opened and read from the file explorer to read the strings.
     * - Rebecca*/
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fName = dialog.getOpenFileName();
    dialog.close();

    QFile f(fName);
    if (!f.open(QIODevice::ReadOnly))
        qDebug() << "NO";
        QErrorMessage err;
        err.showMessage("The file cannot be opened due to permissions access or it is not an ELF file.");
    qDebug() << "you just opened up";
    QByteArray data = f.readAll();

    ui->lineEdit->setText(fName);
    ui->stringTable->clearContents();
    ui->stringTable->setSortingEnabled(false);
    ui->stringTable->setRowCount(0);
    ui->sectTable->clearContents();
    ui->sectTable->setColumnCount(0);

    this->FileStrings(data, data.length());
    this->FileHeader(data[4]);
    float e = this->FileEntropy(data, data.length());
    ui->lineEdit_2->setText(QString::number(data.length()) + " bytes");
    ui->lineEdit_7->setText(QString::number(e, 'g', 4));

    QString h1 = this->CalculateHash(data, data.length(), EVP_md5(), MD5_DIGEST_LENGTH);
    QString h2 = this->CalculateHash(data, data.length(), EVP_sha1(), SHA_DIGEST_LENGTH);
    QString h3 = this->CalculateHash(data, data.length(), EVP_sha256(), SHA256_DIGEST_LENGTH);
    ui->lineEdit_4->setText(h1);
    ui->lineEdit_5->setText(h2);
    ui->lineEdit_6->setText(h3);

    this->Load_Sections(fName);
    f.close();
}

void Framework::FileHeader(int b){
    /*Is the ELF file 32-bit or 64-bit based on the file header?
     * - Rebecca*/
    qDebug() << b;
    if(b == 1){
        ui->lineEdit_3->setText("32 Bit");
    }

    else {
        ui->lineEdit_3->setText("64 Bit");
    }
}

template <typename T>
float Framework::FileEntropy(T d, int sz){
    /*Entropy  of file
     - Rebecca*/
    std::map<int,int> freq;

    for(int i = 0; i < sz; i++){
        int count = (int)(d[i]);
        freq[count]++;
    }
    float entropy = 0;
    for(auto i = freq.begin(); i != freq.end(); i++){
        float p = (float)i->second / (float)sz;
        if (p > 0){
              entropy += p * log2(p);
         }
    }
    entropy *= -1;
    qDebug() << entropy;
    return entropy;

}

template <typename T>
QString Framework::CalculateHash(T d, int sz, const EVP_MD* evp, int len){
    /*Calculates the hashes supposedly.
     * - Rebecca*/
    //qDebug() << d;
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

void Framework::FileStrings(QByteArray d, int sz){
  char buf[512];		/* the strings buffer */
  char *bufptr;			/* pointer into the strings buffer */
  long offset = 0L;			/* file offset */
  long limit = 0L;			/* limit, if doing data segment only */
  uint8_t c;			/* input character */
  bool unicode = false;

  bufptr = buf;
  while (offset < sz) {
    c = d[(int)offset];
    if ((('\0' == c || '\n' == c) && bufptr - buf >= 4)
        || (bufptr - buf == 512 - 1)) {
        *bufptr = '\0';
        ui->stringTable->insertRow(ui->stringTable->rowCount());
        if(unicode == true){
            std::cout << buf << std::endl;
            ui->stringTable->setItem(ui->stringTable->rowCount()-1, 0, new QTableWidgetItem("Unicode"));
            unicode = false;
        }
        else{
            ui->stringTable->setItem(ui->stringTable->rowCount()-1, 0, new QTableWidgetItem("Ascii"));
        }
        ui->stringTable->setItem(ui->stringTable->rowCount()-1, 1, new QTableWidgetItem(QString::number(strlen(buf))));
        ui->stringTable->setItem(ui->stringTable->rowCount()-1, 2, new QTableWidgetItem(buf));
        bufptr = buf;
    } else if (' ' <= c || '\t' == c){
        if(c > 177){
            unicode = true;
        }
        *bufptr++ = c;
    }
    else
        bufptr = buf;

    ++offset;
  }
  ui->stringTable->setSortingEnabled(true);
}

/*
    const char *section_name;
    asection *bfd_section;
    Section *section;

    uint64_t size, vma;
    std::string name;

    Section::Section_Type sec_type;

    //loop through all of the sections
    for(bfd_section = bfd_h -> sections; bfd_section;bfd_section -> next)
    {
        sec_type = Section::NONE;

        binary -> sections.push_back(Section());
        section = &binary -> sections.back();

        vma = bfd_section_vma(bfd_section);
        size = bfd_section_size(bfd_section);
        name = bfd_section_name(bfd_section);

        section -> set_section_vma(vma);
        section -> set_section_size(size);
        section -> set_section_name(name);
        section -> set_section_bytes((uint8_t*)malloc(section -> get_section_size(bfd_section_size(bfd_section))));

    }
    return 0; //successfully loaded
*/

void Framework::Load_Sections(QString filename)
{
    unsigned int magic = bfd_init();
    char* target = NULL;

    bfd* bfd_h = bfd_openr(filename.toStdString().c_str(), NULL);

    std::cout << "Printing the BFD_sections: " << bfd_h -> sections;

    if(!bfd_h)
        std::cout << "Failed to read bfd file \n";
        QErrorMessage err;
        err.showMessage("Failed to read bfd file.");

    int check = bfd_check_format(bfd_h, bfd_object);
    if(check == 0)
        std::cout << "BFD did not load";

    ufile_ptr ufile_size;

    std::string flavor = bfd_h -> xvec -> name;

    std::cout << "The flavor of this file is: " << flavor << "\n";

    const char* section_name;

    asection *bfd_sec;

    std::cout << "The bfd file name is: " << bfd_h -> filename;

    uint64_t entry = bfd_get_start_address(bfd_h);

    printf("C style entry@0x%016jx\n", entry);

    ui->lineEdit_8->setText("0x" + QString::number(entry));

    std::cout << "The last section in the bfd is: " << bfd_h -> section_last << "\n";

    unsigned int s_count = bfd_get_symcount(bfd_h);

    std::cout << "The number of symbols is: " << s_count << " \n";

    const bfd_arch_info_type *bfd_arch_info;
    bfd_arch_info = bfd_get_arch_info(bfd_h);
    std::string bfd_arch_string = std::string(bfd_arch_info -> arch_name);

    int bits = bfd_arch_info -> bits_per_address;

    std::cout << "The bits per address is: " << bits << " \n";

    std::cout << "The printable arch info is: " << bfd_arch_string << " \n";




    //Loading the symbols first;
    long n, number_syms;
    n = bfd_get_symtab_upper_bound(bfd_h);
    if(n < 0)
    {
        std::cout << "Failed to read the symbol table \n";
        std::cout << bfd_errmsg(bfd_get_error()) << "\n";
        QErrorMessage err;
        err.showMessage("Failed to read the symbol table. The file is likely stripped.");
    } else
    {
        std::cout << "Value of n: " << n << "\n";
    }

    std::vector<std::string> symbols;
    asymbol **bfd_symtab;
    bfd_symtab = NULL;
    bfd_symtab = (asymbol**)malloc(n);
    number_syms = bfd_canonicalize_symtab(bfd_h, bfd_symtab);
    std::cout << "Number of symbols: " << number_syms << std::endl;


    for(int i = 0; i < number_syms; i++)
    {
        if(bfd_symtab[i] -> flags & BSF_FUNCTION)
        {
            symbols.push_back(std::string(bfd_symtab[i] -> name));
        }
    }

    std::cout << "The size of the symbols vector: " << symbols.size() << std::endl;

    for(int i = 0; i < symbols.size(); i++)
    {
        std::cout << symbols[i] << std::endl;
    }


    std::vector<std::string> sections;
    uint64_t size;
    uint64_t r_size;
    uint8_t *bytes;
    unsigned int number = bfd_h -> id;
    std::cout << "The ID of the bfd is: " << number << std::endl;
    for(bfd_sec = bfd_h -> sections; bfd_sec; bfd_sec = bfd_sec -> next)
    {
        section_name = bfd_section_name(bfd_sec);
        if(!section_name)
            section_name = "No_name";

        std::string name = std::string(section_name);

        size = bfd_section_size(bfd_sec);
        r_size = bfd_section_rsize(bfd_sec);

        sections.push_back(name);
        bytes  = (uint8_t*)malloc(size);

        std::string v_addr = "0x" + std::to_string(bfd_section_vma(bfd_sec));
        std::string l_addr = "0x" + std::to_string(bfd_section_lma(bfd_sec));

        uint8_t* buf;
        buf = (uint8_t*)malloc(size);
        if(!bfd_get_section_contents(bfd_h, bfd_sec, buf, 0, size)){
                std::cout << "Unable to read " << name << " due to unexpected error." << std::endl;
        }
        std::cout << *buf << "\n";

        if(name == ".text" || name == ".data" || name == ".symtab" || name == ".dynsym" || name == ".dynamic"){
            ui->sectTable->insertColumn(ui->sectTable->columnCount());
            ui->sectTable->setHorizontalHeaderItem(ui->sectTable->columnCount()-1, new QTableWidgetItem("value"));
            ui->sectTable->setItem(0,ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::fromStdString(name)));
            QString h = this->CalculateHash(buf, size, EVP_md5(), MD5_DIGEST_LENGTH);
            float e = this->FileEntropy(buf, size);
            ui->sectTable->setItem(1, ui->sectTable->columnCount()-1, new QTableWidgetItem(h));
            ui->sectTable->setItem(2, ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::number(e)));
            ui->sectTable->setItem(3, ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::number(size)));
            ui->sectTable->setItem(4, ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::fromStdString(v_addr)));
            ui->sectTable->setItem(5, ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::number(r_size)));
            ui->sectTable->setItem(6, ui->sectTable->columnCount()-1, new QTableWidgetItem(QString::fromStdString(l_addr)));
        }

        std::cout << "Section name: " << name << " Size: " << size << " number of bytes: " << bytes << std::endl;
        if (!bfd_get_section_contents(bfd_h, bfd_sec, bytes, 0, size))
        {
            std::cout << "Failed to read section \n";
            QErrorMessage err;
            err.showMessage("Failed to read sections.");
        }
    }

    std::cout << "Size of the sections vector: " << sections.size() << "\n";

    bfd_close(bfd_h);
}
