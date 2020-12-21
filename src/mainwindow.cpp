#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    addMenuForSort();
    connect(ui->add, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
}

void MainWindow::addButtonClicked(){
    courtCaseWindow = new CourtCaseWindow(db, this);

    courtCaseWindow->show();
    courtCaseWindow->setGeometry(0, 29, this->width(), this->height()-29);
}

void MainWindow::addMenuForSort() {
    auto sortMenu = new QMenu(this);
    sortMenu->installEventFilter(this);
    ui->sort->setMenu(sortMenu);
    sortMenu->setMaximumWidth(120);

    QAction *action = new QAction("СОРТИРОВАТЬ ПО:");
    sortMenu->addAction(action);
    action->setDisabled(true);

    QAction *action2 = new QAction("ИМЯ");
    sortMenu->addAction(action2);
    connect(action2, SIGNAL(triggered()), this, SLOT (sortActionTriggired()));

    QAction *action3 = new QAction("ДАТА");
    sortMenu->addAction(action3);
    connect(action3, SIGNAL(triggered()), this, SLOT (sortActionTriggired()));

    QAction *action4 = new QAction("ПРИОРИТЕТ");

    sortMenu->addAction(action4);
    connect(action4, SIGNAL(triggered()), this, SLOT (sortActionTriggired()));
    sortMenu->setWindowFlag(Qt::NoDropShadowWindowHint);
    sortMenu->setStyleSheet(StyleHelper::getMenuStyleSheet());
}

bool MainWindow::eventFilter(QObject * obj, QEvent *event) {
    if (event->type() == QEvent::Show && obj == ui->sort->menu()) {
        int menu_x_pos = ui->sort->menu()->pos().x();
        int menu_width = ui->sort->menu()->size().width();
        int button_width = ui->sort->size().width();

        QPoint pos = QPoint(
                    menu_x_pos - menu_width/2 + button_width/2,
                    ui->sort->menu()->pos().y()
                    );
        ui->sort->menu()->move(pos);
        return true;
    }
    return false;
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

void MainWindow::clearStackedWidget() {
    for(int i = ui->stackedWidget->count(); i >= 0; i--)
    {
        QWidget* widget = ui->stackedWidget->widget(i);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
}

void MainWindow::createLatestTemplate(QJsonObject sortedJson) {
    int j = 1;
    for (int i = 0; i < pageButtonList.size(); i++) {
        QGridLayout *pageLayout = new QGridLayout();
        pageLayout->setHorizontalSpacing(30);
        pageLayout->setVerticalSpacing(7);
        int row = 0;
        int column = 0;
        while(j <= templateCount) {
            QJsonObject json = sortedJson.take(QString::number(j)).toObject();
            QPushButton *templateButton = new QPushButton();
            templateButton->setMinimumSize(55, 65);
            templateButton->setMaximumSize(55, 65);
            if (json["is_criminal"].toBool()) {
                templateButton->setStyleSheet(
                            QString("border: none; image: url(:/main/images/main/criminal-protocol.png)")
                            );
            } else {
                templateButton->setStyleSheet(
                            QString(
                                "border: none;"
                                "image: url(:/main/images/main/administrative-protocol-%1.png)"
                                ).arg(json["number_picture"].toInt())
                            );
            }
            QLabel *templateName = new QLabel(json["fullname"].toString());
            templateName->setAlignment(Qt::AlignHCenter);
            templateName->setMaximumWidth(55);
            templateName->setMinimumWidth(55);
            QVBoxLayout *templateLayout = new QVBoxLayout();
            templateLayout->addWidget(templateButton);
            templateLayout->addWidget(templateName);
            templateLayout->setSpacing(5);
            pageLayout->addLayout(templateLayout, row, column);
            j++;
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
    }
}

void MainWindow::addLatestTemplates() {
    clearStackedWidget();
    QListIterator<QString> item({"date", "fullname", "is_criminal"});
    templateCount = db->getLatestTemplates("date").size();
    addPageButtons(templateCount/45 + 1);
    while (item.hasNext()) {
        createLatestTemplate(db->getLatestTemplates(item.next()));
    }
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::pageButtonClicked() {
    pageButtonList[ui->stackedWidget->currentIndex() - buttonDifference]->setStyleSheet("image: url(:/main/images/main/page-switch.png); border:none;");
    QPushButton *currentButton = (QPushButton*)this->sender();
    ui->stackedWidget->setCurrentIndex(pageButtonList.indexOf(currentButton) + buttonDifference);
    currentButton->setStyleSheet("image: url(:/main/images/main/page-switch-hover.png); border:none;");
}

void MainWindow::sortActionTriggired() {
    QAction *currentAction = (QAction*)this->sender();
    if (currentAction->text() == "ДАТА") {
        buttonDifference = 0;
        ui->stackedWidget->setCurrentIndex(0);
    } else if (currentAction->text() == "ИМЯ") {
        buttonDifference = pageButtonList.size();
        ui->stackedWidget->setCurrentIndex(buttonDifference);
    } else if (currentAction->text() == "ПРИОРИТЕТ") {
        buttonDifference = pageButtonList.size() * 2;
        ui->stackedWidget->setCurrentIndex(buttonDifference);
    }

    for (auto item: pageButtonList) {
        item->setStyleSheet("image: url(:/main/images/main/page-switch.png); border:none;");
    }
    pageButtonList[0]->setStyleSheet("image: url(:/main/images/main/page-switch-hover.png); border:none;");
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
