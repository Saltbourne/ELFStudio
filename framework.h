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
#include <QErrorMessage>
#include <QSqlQuery>
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
    void DBConnect();
    void FileOpen();
    void FileHeader(int f);
    template <typename T>
    float FileEntropy(T d, int sz);
    template <typename T>
    QString CalculateHash(T d, int sz, const EVP_MD* evp, int len = 0);
    void FileStrings(QByteArray data, int sz);
    void Load_Sections(QString filename);

private:
    Ui::Framework *ui;
    QSqlDatabase db;
};
#endif // FRAMEWORK_H
