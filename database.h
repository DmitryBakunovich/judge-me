#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

#include <QDebug>

#define DATABASE_PATH "C:\\Study\\judge-me\\judgeme_db"
#define DATABASE_HOSTNAME "JudgeMeDatabase"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase();

private:
    QSqlDatabase db;

private:
    bool openDataBase();
    void closeDataBase();
};

#endif // DATABASE_H
