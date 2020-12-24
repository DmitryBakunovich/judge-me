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
#include <QList>

#define DATABASE_PATH "C:\\Study\\judge-me\\judgeme.db"
#define DATABASE_HOSTNAME "JudgeMeDatabase"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();

    void connectToDataBase();

    QJsonArray getFieldsForJudgment(QString article);
    QList<QString> getAllJudgment();
    QMap<QString, QString> getAllFields();
    QJsonObject getLatestTemplates(QString sortBy);
    QString getTextTemplate(QString article);
    bool getLegalResponsibility(QString article);

    void addJudgment(QString article, QJsonDocument fields);
    void updateJudgment(QString article, QJsonDocument fields, QString text);
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
