#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QDateTime>
#include <QSignalMapper>

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
    ui->mainLayout->setSpacing(windowWidth/8);

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

    //qsrand(QTime::currentTime().msecsSinceStartOfDay());

    addLatestTemplates();
}

void MainWindow::addPageButtons(int buttonsCount) {
    QSpacerItem *upperSpacer = new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    ui->verticalLayout->addItem(upperSpacer);
    for (int i = 0; i < buttonsCount; i++) {
        QPushButton *pageButton = new QPushButton();
        pageButton->setMinimumSize(8, 8);
        pageButton->setMaximumSize(8, 8);
        if (i == buttonsCount - 1) {
            pageButton->setStyleSheet("image: url(:/main/images/main/page-switch-hover.png); border:none;");
        } else {
            pageButton->setStyleSheet("image: url(:/main/images/main/page-switch.png); border:none;");
        }
        ui->verticalLayout->addWidget(pageButton);
        pageButtonList.prepend(pageButton);
        connect(pageButton, SIGNAL(clicked()), this, SLOT (pageButtonClicked()));
    }
    QSpacerItem *lowerSpacer = new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    ui->verticalLayout->addItem(lowerSpacer);
    ui->verticalLayout->setSpacing(7);
}

void MainWindow::cleanStackedWidget() {
    for(int i = ui->stackedWidget->count(); i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
}

void MainWindow::addLatestTemplates() {
    cleanStackedWidget();
    QJsonObject allLatestTemplates = db->getLatestTemplates();
    int templateCount = allLatestTemplates.size();
    addPageButtons(templateCount/45 + 1);
    for (int i = 0; i < pageButtonList.size(); i++) {
        QGridLayout *pageLayout = new QGridLayout();
        pageLayout->setHorizontalSpacing(30);
        pageLayout->setVerticalSpacing(7);
        int row = 0;
        int column = 0;
        while(templateCount) {
            QJsonObject jsonArray = allLatestTemplates.take(QString::number(templateCount)).toObject();
            QPushButton *templateButton = new QPushButton();
            templateButton->setMinimumSize(55, 65);
            templateButton->setMaximumSize(55, 65);
            if (jsonArray["is_criminal"].toBool()) {
                templateButton->setStyleSheet(
                            QString("border: none; image: url(:/main/images/main/criminal-protocol.png)")
                            );
            } else {
                templateButton->setStyleSheet(
                            QString(
                                "border: none;"
                                "image: url(:/main/images/main/administrative-protocol-%1.png)"
                                ).arg(jsonArray["number_picture"].toInt())
                            );
            }
            QLabel *templateName = new QLabel(jsonArray["fullname"].toString());
            templateName->setAlignment(Qt::AlignHCenter);
            templateName->setMaximumWidth(55);
            templateName->setMinimumWidth(55);
            QVBoxLayout *templateLayout = new QVBoxLayout();
            templateLayout->addWidget(templateButton);
            templateLayout->addWidget(templateName);
            templateLayout->setSpacing(5);
            pageLayout->addLayout(templateLayout, row, column);
            templateCount -= 1;
            if (column == 10) {
                if (row == 3) {
                    row = 0;
                    column = 0;
                    break;
                }
                else {
                    column = 0;
                    row += 1;
                    continue;
                }
            }
            column += 1;
        }
        pageLayout->setContentsMargins(0, 40, 0, 0);
        QSpacerItem *downSpacer = new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
        pageLayout->addItem(downSpacer, 5, 0);
        QWidget *pageWidget = new QWidget();
        ui->stackedWidget->addWidget(pageWidget);
        pageWidget->setLayout(pageLayout);
        pageList.append(pageWidget);
    }
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::pageButtonClicked() {
    qDebug() << true;
}

QPoint MainWindow::previousPosition() const {
    return mousePreviousPosition;
}

void MainWindow::setPreviousPosition(QPoint previousPosition) {
    if (mousePreviousPosition == previousPosition)
        return;
    mousePreviousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && event->pos().y() <= 29 && event->pos().y() >= 0) {
        isMoveWindow = true;
        setPreviousPosition(event->pos());
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

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isMoveWindow = false;
    }
    return QWidget::mouseReleaseEvent(event);
}

MainWindow::~MainWindow() {
    delete ui;
}
