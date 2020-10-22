#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new DataBase();
    db->connectToDataBase();
    qDebug() << db->takeAllFields();
    qDebug() << db->takeFieldsForJudgment();
}

MainWindow::~MainWindow()
{
    delete ui;
}
