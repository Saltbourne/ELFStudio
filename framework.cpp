#include "framework.h"
#include "ELFStudio_autogen/include/ui_framework.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Framework::Framework(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Framework)
{
    ui->setupUi(this);
    ui->stringTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QObject::connect(ui->openButton, &QAction::triggered, this, &Framework::FileOpen);
}

Framework::~Framework()
{
    delete ui;
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
    qDebug() << "you just opened up";
    QByteArray data = f.readAll();

    ui->lineEdit->setText(fName);
    ui->stringTable->clearContents();
    ui->stringTable->setRowCount(0);
    this->Ascii_Strings(data, data.length());
    //this->Unicode_Strings(data, data.length());
    this->FileHeader(data[4]);
    this->FileEntropy(data, data.length());
    ui->lineEdit_2->setText(QString::number(data.length()) + " bytes");
    QString h1 = this->CalculateHash(data, data.length(), EVP_md5(), MD5_DIGEST_LENGTH);
    QString h2 = this->CalculateHash(data, data.length(), EVP_sha1(), SHA_DIGEST_LENGTH);
    QString h3 = this->CalculateHash(data, data.length(), EVP_sha256(), SHA256_DIGEST_LENGTH);
    ui->lineEdit_4->setText(h1);
    ui->lineEdit_5->setText(h2);
    ui->lineEdit_6->setText(h3);
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

void Framework::FileEntropy(QByteArray d, int sz){
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
    ui->lineEdit_7->setText(QString::number(entropy, 'g', 4));

}

QString Framework::CalculateHash(QByteArray d, int sz, const EVP_MD* evp, int len){
    /*Calculates the hashes supposedly.
     * - Rebecca*/
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

void Framework::Ascii_Strings(QByteArray d, int sz){
  char buf[512];		/* the strings buffer */
  char *bufptr;			/* pointer into the strings buffer */
  long offset = 0L;			/* file offset */
  long limit = 0L;			/* limit, if doing data segment only */
  int c;			/* input character */
  bool unicode = false;

  bufptr = buf;
  while (offset < sz) {
    c = d[(int)offset];
    if ((('\0' == c || '\n' == c) && bufptr - buf >= 4)
        || (bufptr - buf == 512 - 1)) {
        *bufptr = '\0';
        ui->stringTable->insertRow(ui->stringTable->rowCount());
        if(unicode == true){
            ui->stringTable->setItem(ui->stringTable->rowCount()-1, 0, new QTableWidgetItem("Unicode"));
            unicode = false;
        }
        else{
            ui->stringTable->setItem(ui->stringTable->rowCount()-1, 0, new QTableWidgetItem("Ascii"));
        }
        ui->stringTable->setItem(ui->stringTable->rowCount()-1, 1, new QTableWidgetItem(QString::number(strlen(buf))));
        ui->stringTable->setItem(ui->stringTable->rowCount()-1, 2, new QTableWidgetItem(buf));
        bufptr = buf;
    } else if (' ' <= c || '\t' == c || c < 0){
        if(c < 0){
            unicode = true;
        }
        *bufptr++ = c;
    }
    else
        bufptr = buf;

    ++offset;
  }
}

void Framework::Unicode_Strings(QByteArray d, int sz){
  char buf[512];		/* the strings buffer */
  char *bufptr;			/* pointer into the strings buffer */
  long offset = 0L;			/* file offset */
  long limit = 0L;			/* limit, if doing data segment only */
  int c;			/* input character */

  bufptr = buf;

  while (offset < sz) {
    c = d[(int)offset];
    if ((('\0' == c || '\n' == c) && bufptr - buf >= 4)
        || (bufptr - buf == 512 - 1)) {
        *bufptr = '\0';
        ui->stringTable->insertRow(ui->stringTable->rowCount());
        ui->stringTable->setItem(ui->stringTable->rowCount()-1, 0, new QTableWidgetItem("Unicode"));
        ui->stringTable->setItem(ui->stringTable->rowCount()-1, 1, new QTableWidgetItem(QString::number(strlen(buf))));
        ui->stringTable->setItem(ui->stringTable->rowCount()-1, 2, new QTableWidgetItem(buf));
        bufptr = buf;
    } else if (' ' <= abs(c) || '\t' == c || c < 0){
        *bufptr++ = c;
        qDebug() << buf;}
    else
        bufptr = buf;

    ++offset;
  }
}
