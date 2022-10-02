#include "framework.h"
#include "ELFStudio_autogen/include/ui_framework.h"
#include "ui_framework.h"

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
    /*Any ELF file can now be opened and read from the file explorer to read the strings.
     * - Rebecca*/
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QString fName = dialog.getOpenFileName();
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
    QByteArray data = f2.read(f2.size());
    this->FileHeader(data[4]);
    this->CalculateMD5(data, f2.size());
    f2.close();
}

void Framework::FileHeader(int b){
    /*Is the ELF file 32-bit or 64-bit based on the file header?
     * - Rebecca*/
    qDebug() << b;
    if(b == 1){
        qDebug() << "32 Bit";
    }

    else {
        qDebug() << "64 Bit";
    }
}

void Framework::CalculateMD5(QByteArray d, int sz){
    /*Calculates the MD5 hash supposedly.
     * - Rebecca*/
    QString hash;
    char buf[32];
    for(int i = 0; i < sz; i++){
        sprintf(buf, "%02x", d[i]);
        hash.append(buf);
    }
    qDebug() << hash;
}
