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
    /*Trying to read example strings from an example binary when 'opening' a file, but permission issues galore.
     * Developing this on Windows 10 too since I am waiting on VMWare license to be renewed. Stay tuned.
        - Rebecca*/
    //std::ofstream f;
    //f.open("./bbbbloat");
    //f.close();
    //system("strings bbbbloat");
    //std::ifstream stringsFile;
    //std::string line;
    //stringsFile.open("./strings.txt");
    //while (std::getline(stringsFile, line))
    //{
        //qDebug() << "WORK";
        //qDebug() << &line ;
    //}
    //stringsFile.close();
    /*
    qDebug() << QDir::currentPath();
    QFile f("strings.txt");
    if (!f.open(QIODevice::ReadOnly))
        qDebug() << "NO";
        return;
    qDebug() << "you just opened up";
    QTextStream stringsFile(&f);
    for (QString line = stringsFile.readLine();
         !line.isNull();
         line = stringsFile.readLine()) {
         qDebug() << "WORK";
         qDebug() << &line ;
    };
    */
    /*Okay this is just to test that adding the strings actually works the way it is intended in the GUI
        - Rebecca*/
    QString test[3] = {"test1", "test2", "test3"};
    for(int i = 0; i < sizeof(test)/sizeof(test[0]); i++){

         ui->stringsBox->append(test[i] + "\n");
    }
}

