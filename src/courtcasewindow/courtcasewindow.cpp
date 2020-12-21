#include "courtcasewindow.h"
#include "ui_courtcasewindow.h"

CourtCaseWindow::CourtCaseWindow(DataBase *dbMainWindow, QWidget *parent) : QDialog(parent), ui(new Ui::CourtCaseWindow) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->close, &QToolButton::clicked, this, &QWidget::close);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeComboBox(int)));

    connect(ui->firstPageButton, SIGNAL(clicked()), this, SLOT(firstPageClicked()));
    connect(ui->secondPageButton, SIGNAL(clicked()), this, SLOT(secondPageClicked()));
    connect(ui->okeyButtun, SIGNAL(clicked()), this, SLOT(okeyButtonClicked()));

    db = dbMainWindow;
    for(auto it: db->getAllJudgment()) {
        ui->comboBox->addItem(it);
    }

    allFields = db->getAllFields();
}

void CourtCaseWindow::changeMainPageStyle() {
    if (ui->stackedWidget->currentIndex() == 0) {
        ui->mainWidget->setStyleSheet(StyleHelper::getFirstPageStyle());
        ui->mainWidget->setMinimumSize(549, 401);
        ui->mainWidget->setMaximumSize(549, 401);
    } else {
        ui->mainWidget->setStyleSheet(StyleHelper::getSecondPageStyle());
        ui->firstPageButton->setStyleSheet(StyleHelper::getFirstPageButtonStyle());
        ui->secondPageButton->setStyleSheet(StyleHelper::getSecondPageButtonStyle());
        ui->secondPageButton->setMinimumSize(53, 53);
        ui->mainWidget->setMinimumSize(549, 614);
        ui->mainWidget->setMaximumSize(549, 614);
    }
}

void CourtCaseWindow::firstPageClicked() {
    ui->stackedWidget->setCurrentIndex(0);
    changeMainPageStyle();
}

void CourtCaseWindow::secondPageClicked() {
    ui->stackedWidget->setCurrentIndex(1);
    changeMainPageStyle();
}

bool CourtCaseWindow::checkFillingFields() {
    bool isEmptyField = false;
    int firstEmptyIndex = 0;
    for (auto item: fieldsList) {
        if (item->text() == "") {
            item->setStyleSheet(StyleHelper::getEmptyFieldStyle());
            if (!isEmptyField) {
                firstEmptyIndex = fieldsList.indexOf(item);
                ui->stackedWidget_2->setCurrentIndex(firstEmptyIndex/6);
            }
            isEmptyField = true;
        } else {
            item->setStyleSheet(StyleHelper::getFillFieldStyle());
        }
    }
    return isEmptyField;
}

void CourtCaseWindow::createFile(QString text) {
    QString pathToLastFileDir = QCoreApplication::applicationDirPath() + "/lastfile/";
    QDir lastFileDir = pathToLastFileDir;
    if (!lastFileDir.exists()) {
        lastFileDir.mkdir(pathToLastFileDir);
    }
    QString filename(ui->filenameEdit->text() + " " + ui->comboBox->currentText());
    QFile file(pathToLastFileDir + filename + ".docx");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream fileout(&file);
        fileout << text;
        file.close();
    }
}

void CourtCaseWindow::createCourtCase() {
    if (!checkFillingFields()) {
        QString text = db->getTextTemplate(ui->comboBox->currentText());
        for (auto item: fieldsList) {
            text.replace(allFields.key(item->placeholderText()), item->text());
        }
        createFile(text);
        this->close();
    }
}

void CourtCaseWindow::okeyButtonClicked() {
    if (ui->stackedWidget->currentIndex() == 0 &&
        ui->comboBox->currentText() != "" &&
        ui->filenameEdit->text() != "") {
        ui->secondPageButton->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(1);
        changeMainPageStyle();
    } else if (ui->stackedWidget->currentIndex() == 1) {
        createCourtCase();
    }
}

void CourtCaseWindow::clearStackedWidget() {
    for(int i = ui->stackedWidget_2->count(); i >= 0; i--) {
        QWidget* widget = ui->stackedWidget_2->widget(i);
        ui->stackedWidget_2->removeWidget(widget);
        widget->deleteLater();
    }
}

void CourtCaseWindow::makePageButtons(int buttonsCount) {
    QSpacerItem *leftSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->buttonLayout->addItem(leftSpacer);

    QPushButton *slideLeftButton = new QPushButton();
    slideLeftButton->setStyleSheet(StyleHelper::getLeftSlideStyle());
    slideLeftButton->setMinimumSize(12, 12);
    slideLeftButton->setMaximumSize(12, 12);
    ui->buttonLayout->addWidget(slideLeftButton);
    pageButtonList.append(slideLeftButton);
    connect(slideLeftButton, SIGNAL(clicked()), this, SLOT(slideButtonClicked()));
    slideLeftButton->setVisible(false);

    for (int i = 0; i < buttonsCount; i++) {
        QPushButton *pageButton = new QPushButton();
        pageButton->setMinimumSize(8, 8);
        pageButton->setMaximumSize(8, 8);
        if (i == 0) {
            pageButton->setStyleSheet(
                        "image: url(:/courtcasewindow/images/courtcasewindow/page-button-switch.png);"
                        "border:none;");
        } else {
            pageButton->setStyleSheet(
                        "image: url(:/courtcasewindow/images/courtcasewindow/page-button.png);"
                        "border:none;");
        }
        ui->buttonLayout->addWidget(pageButton);
        if (i >= 3) {
            pageButton->setVisible(false);
        }
        pageButtonList.append(pageButton);
        connect(pageButton, SIGNAL(clicked()), this, SLOT (pageButtonClicked()));
    }

    QPushButton *slideRightButton = new QPushButton();
    slideRightButton->setStyleSheet(StyleHelper::getRightSlideStyle());
    slideRightButton->setMinimumSize(12, 12);
    slideRightButton->setMaximumSize(12, 12);
    ui->buttonLayout->addWidget(slideRightButton);
    pageButtonList.append(slideRightButton);
    (buttonsCount > 4) ? slideRightButton->setVisible(true) : slideRightButton->setVisible(false);
    connect(slideRightButton, SIGNAL(clicked()), this, SLOT(slideButtonClicked()));

    ui->buttonLayout->setSpacing(7);
    QSpacerItem *rightSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->buttonLayout->addItem(rightSpacer);
}

void CourtCaseWindow::clearButtonsLayout(){
    QLayoutItem *item;
    while((item = ui->buttonLayout->takeAt(0))) {
        delete item->widget();
    }
}

void CourtCaseWindow::changeComboBox(int index) {
    QString caseName = ui->comboBox->itemText(index);

    QJsonArray fieldsArray = db->getFieldsForJudgment(caseName);
    clearStackedWidget();
    clearButtonsLayout();
    pageButtonList.clear();
    fieldsList.clear();
    makePageButtons(fieldsArray.size()/6 + 1);
    for (int i = 0; i < pageButtonList.size(); i++) {
        QVBoxLayout *pageLayout = new QVBoxLayout();
        pageLayout->setSpacing(10);
        for(int itemInPage = 0; itemInPage < 6; itemInPage++) {
            if (!fieldsArray.isEmpty()) {
                QLineEdit *fieldEdit = new QLineEdit();
                fieldEdit->setMinimumHeight(40);
                fieldEdit->setStyleSheet(StyleHelper::getFieldStyle());
                fieldEdit->setPlaceholderText(
                            allFields[fieldsArray.first().toString()]
                        );
                fieldsArray.pop_front();
                fieldsList.append(fieldEdit);
                pageLayout->addWidget(fieldEdit);
            }
            else {
                break;
            }
        }
        QSpacerItem *lowerSpacer = new QSpacerItem(1,1, QSizePolicy::Fixed, QSizePolicy::Expanding);
        pageLayout->addItem(lowerSpacer);

        QWidget *pageWidget = new QWidget();
        ui->stackedWidget_2->addWidget(pageWidget);
        pageWidget->setLayout(pageLayout);
    }
}

void CourtCaseWindow::pageButtonClicked() {
    for (int i = 1; i < pageButtonList.size() - 1; i++) {
        pageButtonList[i]->setStyleSheet(
                    "image: url(:/courtcasewindow/images/courtcasewindow/page-button.png);"
                    "border:none;");
    }
    QPushButton *currentButton = (QPushButton*)this->sender();
    ui->stackedWidget_2->setCurrentIndex(pageButtonList.indexOf(currentButton) - 1);
    currentButton->setStyleSheet(
                "image: url(:/courtcasewindow/images/courtcasewindow/page-button-switch.png);"
                "border:none;");
}

void CourtCaseWindow::slideButtonClicked() {
    QPushButton *currentButton = (QPushButton*)this->sender();
    if (currentButton == pageButtonList.last()) {
        buttonDifference += 1;
        pageButtonList[3 + buttonDifference]->setVisible(true);
        pageButtonList[buttonDifference]->setVisible(false);
    } else if (currentButton == pageButtonList.first()) {
        pageButtonList[3 + buttonDifference]->setVisible(false);
        pageButtonList[buttonDifference]->setVisible(true);
        buttonDifference -= 1;
    }

    if (buttonDifference == 0) {
        pageButtonList.first()->setVisible(false);
    } else if (buttonDifference == pageButtonList.size() - 5) {
        pageButtonList.last()->setVisible(false);
    } else {
        pageButtonList.first()->setVisible(true);
        pageButtonList.last()->setVisible(true);
    }
}

CourtCaseWindow::~CourtCaseWindow() {
    delete ui;
}
