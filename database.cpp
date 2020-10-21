#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent) {

}

DataBase::~DataBase() {

}

void DataBase::connectToDataBase() {
    if(QFile(DATABASE_PATH).exists()) {
        this->openDataBase();
    }
}

bool DataBase::openDataBase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_PATH);
    if(db.open()) {
        return true;
    } else {
        return false;
    }
}

void DataBase::closeDataBase() {
    db.close();
}

QJsonArray DataBase::takeFieldsForJudgment() {
    QSqlQuery query;
    query.exec("SELECT fields FROM judgment");
    query.first();
    QByteArray jsonBuffer = query.value(0).toByteArray();
    auto json = QJsonDocument::fromJson(jsonBuffer);
    QJsonObject jsonObject = json.object();
    QJsonArray jsonArray = jsonObject["fields_array"].toArray();
    return jsonArray;
}

QMap<QString, QString> DataBase::takeAllFields() {
    QSqlQuery query;
    QMap<QString, QString> allFieldsMap;
    query.exec("SELECT name, symbol FROM field");
    while (query.next()) {
        allFieldsMap.insert(
                    query.value(0).toString(),
                    query.value(1).toString()
                    );
    }
    return allFieldsMap;
}

void DataBase::addJudgment(QString article, QJsonDocument fields) {
    QSqlQuery query;
    query.prepare("INSERT INTO judgment (article, fields) VALUES (:article, :fields)");
    query.bindValue(":article", article);
    query.bindValue(":fields", fields.toJson());
    query.exec();
    qDebug() << true;
}

void DataBase::deleteJudgment(QString article) {
    QSqlQuery query;
    query.prepare("DELETE FROM judgment WHERE article = :article ");
    query.bindValue(":article", article);
    query.exec();
    qDebug() << true;
}
