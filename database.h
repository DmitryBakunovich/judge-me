#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMap>
#include <QDebug>

#define DATABASE_PATH "C:\\Study\\judge-me\\judgeme.db"
#define DATABASE_HOSTNAME "JudgeMeDatabase"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase();
    QJsonArray takeFieldsForJudgment();
    QMap<QString, QString> takeAllFields();

private:
    QSqlDatabase db;

private:
    bool openDataBase();
    void closeDataBase();
};

#endif
