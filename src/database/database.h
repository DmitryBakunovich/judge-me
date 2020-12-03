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
    QJsonArray getFieldsForJudgment();
    QMap<QString, QString> getAllFields();
    QJsonObject getLatestTemplates();
    void addJudgment(QString article, QJsonDocument fields);
    void updateJudgment(QString article, QJsonDocument fields);
    void deleteJudgment(QString article);
    void addField(QString fieldName, QString fieldReduction);
    void updateField(QString newFieldName, QString fieldReduction);
    void deleteField(QString fieldReduction);

private:
    QSqlDatabase db;

private:
    bool openDataBase();
    void closeDataBase();
};

#endif
