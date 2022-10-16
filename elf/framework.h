#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <QMainWindow>
#include <QObject>
#include <QTextEdit>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QDir>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
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
    void FileHeader(int f);
    QString CalculateHash(QByteArray data, int sz, const EVP_MD* evp, int len = 0);

private:
    Ui::Framework *ui;
};
#endif // FRAMEWORK_H
