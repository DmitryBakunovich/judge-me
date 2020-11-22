#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>
#include <QGraphicsDropShadowEffect>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    db->connectToDataBase();

    this->setWindowFlags(Qt::FramelessWindowHint);  // Disable window decoration

    connect(ui->rollup, &QToolButton::clicked, this, &QWidget::showMinimized);
    connect(ui->close, &QToolButton::clicked, this, &QWidget::close);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(5); // Setting shadow blur
    shadowEffect->setOffset(0); // Setting the tilt of the shadow
    ui->centralwidget->setGraphicsEffect(shadowEffect);

    QList <QScreen *> screens = QGuiApplication::screens();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    qDebug() << height;
    qDebug() << width;

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
