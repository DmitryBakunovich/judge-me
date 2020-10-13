#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase()
{
    if(QFile(DATABASE_PATH).exists()){
        this->openDataBase();
    }
}

bool DataBase::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName("C:\\Study\\judge-me\\judgeme_db");
    if(db.open()){
        qDebug() << true;
        return true;
    } else {
        return false;
    }
}

void DataBase::closeDataBase()
{
    db.close();
}
