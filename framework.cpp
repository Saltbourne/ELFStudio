#include "framework.h"
#include "ELFStudio_autogen/include/ui_framework.h"

Framework::Framework(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Framework)
{
    ui->setupUi(this);
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
    ui->lineEdit->setText(fName);
    dialog.close();
    std::string command = "strings "+fName.toStdString()+" > strings.txt";
    system(command.c_str());
    qDebug() << QDir::currentPath();
    QFile f(QString(QCoreApplication::applicationDirPath() + "/strings.txt"));
    if (!f.open(QIODevice::ReadOnly))
        qDebug() << "NO";
    qDebug() << "you just opened up";
    QTextStream stringsFile(&f);
    for (QString line = stringsFile.readLine();
         !line.isNull();
         line = stringsFile.readLine()) {
         ui->stringsBox->append(line + "\n");
    };
    f.close();

    QFile f2(fName);
    if (!f2.open(QIODevice::ReadOnly))
        qDebug() << "NO STILL";
    qDebug() << "you opened up again? ugh";
    QByteArray data = f2.readAll();
    this->FileHeader(data[4]);
    this->FileEntropy(data, data.length());
    ui->lineEdit_2->setText(QString::number(data.length()) + " bytes");
    QString h1 = this->CalculateHash(data, data.length(), EVP_md5(), MD5_DIGEST_LENGTH);
    QString h2 = this->CalculateHash(data, data.length(), EVP_sha1(), SHA_DIGEST_LENGTH);
    QString h3 = this->CalculateHash(data, data.length(), EVP_sha256(), SHA256_DIGEST_LENGTH);
    ui->lineEdit_4->setText(h1);
    ui->lineEdit_5->setText(h2);
    ui->lineEdit_6->setText(h3);
    f2.close();
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
