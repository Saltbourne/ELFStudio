#include "framework.h"
#include <QApplication>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QQuickStyle::setStyle("Material");
    Framework w;
    w.show();
    return a.exec();
}
