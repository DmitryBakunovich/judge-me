#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    this->setFixedSize(width/1.5, height/1.44);

    db->connectToDataBase();

    this->setWindowFlags(Qt::FramelessWindowHint);  // Disable window decoration
    connect(ui->rollup, &QToolButton::clicked, this, &QWidget::showMinimized);
    connect(ui->close, &QToolButton::clicked, this, &QWidget::close);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(5); // Setting shadow blur
    shadowEffect->setOffset(0); // Setting the tilt of the shadow
    ui->centralwidget->setGraphicsEffect(shadowEffect);

    int windowHeight = this->height();
    int windowWidth = this->width();

    // Resize mainLayout
    ui->mainLayout->setContentsMargins(0, 0, 40, windowHeight/2.6);
    ui->mainLayout->setSpacing(windowWidth/9);

    // Resize mainMenuLayout
    ui->mainMenuLayout->setContentsMargins(windowWidth/16.4, windowHeight/21.6, 0, 0);
    ui->mainMenuLayout->setSpacing(windowHeight/30);

    // Resize main menu button
    int mainMenuButtonSize = windowHeight/12;
    ui->add->setMinimumSize(mainMenuButtonSize, mainMenuButtonSize);
    ui->edit->setMinimumSize(mainMenuButtonSize, mainMenuButtonSize);
    ui->trashBasket->setMinimumSize(mainMenuButtonSize, mainMenuButtonSize);
    ui->label_3->setMaximumSize(mainMenuButtonSize/2, 43);
    ui->label_3->setText("12..");
}

QPoint MainWindow::previousPosition() const
{
    return mousePreviousPosition;
}

void MainWindow::setPreviousPosition(QPoint previousPosition)
{
    if (mousePreviousPosition == previousPosition)
        return;
    mousePreviousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // При клике левой кнопкой мыши
    if (event->button() == Qt::LeftButton && event->pos().y() <= 29 && event->pos().y() >= 0) {
        isMoveWindow = true;
        setPreviousPosition(event->pos()); // и устанавливаем позицию клика
    }
    return QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isMoveWindow) {
        auto dx = event->x() - mousePreviousPosition.x();
        auto dy = event->y() - mousePreviousPosition.y();
        setGeometry(x() + dx, y() + dy, width(), height());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // При отпускании левой кнопки мыши сбрасываем состояние клика
    if (event->button() == Qt::LeftButton) {
        isMoveWindow = false;
    }
    return QWidget::mouseReleaseEvent(event);
}

MainWindow::~MainWindow() {
    delete ui;
}
