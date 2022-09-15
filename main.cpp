#include "framework.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Framework w;
    w.show();
    qDebug() << "you just started up";
    return a.exec();
}
