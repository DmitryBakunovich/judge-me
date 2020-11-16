#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include <QGraphicsDropShadowEffect>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint); // Отключаем оформление окна

    connect(ui->svernut, &QToolButton::clicked, this, &QWidget::showMinimized);
    connect(ui->maximaz, &QToolButton::clicked, [this](){
        if (this->isMaximized()) {
            ui->maximaz->setStyleSheet(StyleHelper::getRestoreStyleSheet());
            this->showNormal();
        }
        else {
            ui->maximaz->setStyleSheet(StyleHelper::getMaximizeStyleSheet());
            this->showMaximized();
        }
    });
    connect(ui->close, &QToolButton::clicked, this, &QWidget::close);

    db = new DataBase();
    db->connectToDataBase();

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(5);
    shadowEffect->setOffset(0);
    ui->centralwidget->setGraphicsEffect(shadowEffect);
}

MainWindow::~MainWindow()
{
    delete ui;
}
