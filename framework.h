#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <QMainWindow>
#include <QObject>
#include <QTextEdit>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <iostream>
#include <fstream>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class Framework; }
QT_END_NAMESPACE

class Framework : public QMainWindow
{
    Q_OBJECT

public:
    Framework(QWidget *parent = nullptr);
    ~Framework();
    void FileOpen();

private:
    Ui::Framework *ui;
};
#endif // FRAMEWORK_H
