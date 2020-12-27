#include "courtcasewindow.h"
#include "ui_courtcasewindow.h"

#include <QDebug>
#include <QListView>

CourtCaseWindow::CourtCaseWindow(DataBase *dbMainWindow, QWidget *parent) : QDialog(parent), ui(new Ui::CourtCaseWindow) {
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->close, &QToolButton::clicked, this, &QWidget::close);
    connect(ui->chooseTemplate, SIGNAL(currentIndexChanged(int)), this, SLOT(changeComboBox(int)));

    connect(ui->firstPageButton, SIGNAL(clicked()), this, SLOT(clickedFirstPage()));
    connect(ui->secondPageButton, SIGNAL(clicked()), this, SLOT(clickedSecondPage()));
    connect(ui->okeyButtun, SIGNAL(clicked()), this, SLOT(clickedConfirmButton()));

    db = dbMainWindow;
    for(auto it: db->getAllJudgment()) {
        ui->chooseTemplate->addItem(it);
    }

    allFields = db->getAllFields();
    foreach(QString value, allFields) {
        ui->chooseField->addItem(value);
    }

    connect(ui->editTemplate, SIGNAL(clicked()), this, SLOT(showEditTemplatePage()));
    connect(ui->addTemplate, SIGNAL(clicked()), this, SLOT(showEditTemplatePage()));
    connect(ui->deleteTemplate, SIGNAL(clicked()), this, SLOT(clickedDeleteTemplate()));

    ui->editWidget->setVisible(false);

    QListView *displayChooseTemplate = new QListView();
    ui->chooseTemplate->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui->chooseTemplate->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    ui->chooseTemplate->setView(displayChooseTemplate);

    QListView *displayComboBox = new QListView();
    ui->chooseField->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui->chooseField->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    ui->chooseField->setView(displayComboBox);
}

void CourtCaseWindow::clickedDeleteTemplate() {
    if (ui->chooseTemplate->currentText() != "") {
        db->deleteJudgment(ui->chooseTemplate->currentText());
        ui->chooseTemplate->removeItem(
                    ui->chooseTemplate->currentIndex()
                    );
        ui->chooseTemplate->setCurrentIndex(0);
    } else {
        ui->chooseTemplate->setStyleSheet(StyleHelper::getEmptyComboboxStyle());
    }
}


void CourtCaseWindow::showEditTemplatePage() {
    QPushButton *currentButton = (QPushButton*)this->sender();
    if (ui->chooseTemplate->currentText() == "" && currentButton == ui->editTemplate) {
        return;
    }

    if (currentButton == ui->editTemplate) {
        ui->nameTemplate->setVisible(false);
        ui->whatToDo->setText("Выберите поле и вставьте его в текст");
        ui->textEdit->setText(db->getTextTemplate(
                                  ui->chooseTemplate->currentText()
                                  ));

        QJsonArray templateFields = db->getFieldsForJudgment(
                    ui->chooseTemplate->currentText()
                    );
        templateFieldSet.clear();
        for (auto item : templateFields) {
            templateFieldSet.insert(item.toString());
        }

        if (db->getLegalResponsibility(
                    ui->chooseTemplate->currentText()
                    )) {
            ui->legalResponsibility->setChecked(true);
        }
    } else {
        ui->textEdit->setText("");
    }

    connect(ui->confirmEdit, SIGNAL(clicked()), this, SLOT(clickedConfirmEdit()));
    connect(ui->insertField, SIGNAL(clicked()), this, SLOT(insertFieldInTemplate()));
    connect(ui->deleteField, SIGNAL(clicked()), this, SLOT(clickedDeleteField()));
    connect(ui->closeEditTemplate, SIGNAL(clicked()), this, SLOT(clickedCloseEditTemplate()));
    connect(ui->addField, SIGNAL(clicked()), this, SLOT(clickedAddField()));

    ui->inputField->setVisible(false);
    ui->mainWidget->setVisible(false);
    ui->editWidget->setVisible(true);
}

void CourtCaseWindow::setDisabledField() {
    bool isChecked = ui->addField->isChecked();
    ui->textEdit->setDisabled(isChecked);
    ui->legalResponsibility->setDisabled(isChecked);
    ui->insertField->setDisabled(isChecked);
    ui->deleteField->setDisabled(isChecked);
    ui->nameTemplate->setDisabled(isChecked);
}

void CourtCaseWindow::createField() {
    QString fieldText = ui->inputField->text();
    QString fieldReduction = fieldText.toUpper().left(3);
    qDebug() << fieldReduction;
    int countDuplicate = 0;
    foreach (QString value, allFields) {
         if (QString(allFields.key(value)).mid(1, 3) == fieldReduction) {
            countDuplicate += 1;
         }
    }

    fieldReduction.append(QString::number(countDuplicate));
    fieldReduction = QString("{%1}").arg(fieldReduction);
    if (countDuplicate) {
        fieldText.append(" ");
        fieldText.append(QString::number(countDuplicate));
    }

    db->addField(
                fieldText,
                fieldReduction
                );
    allFields.insert(fieldReduction, fieldText);
    ui->chooseField->addItem(fieldText);
}

void CourtCaseWindow::clickedAddField() {
    if (ui->addField->isChecked()) {
        ui->whatToDo->setText("Введите название нового поля");

        ui->chooseField->setVisible(false);
        ui->inputField->setVisible(true);
        ui->inputField->setStyleSheet(StyleHelper::getEmptyFieldStyle());

        setDisabledField();
    } else {
        QString inputFieldText = ui->inputField->text();
        if (inputFieldText == "" || inputFieldText.length() < 3) {
            ui->addField->setChecked(true);
            return;
        } else {
            createField();

            ui->chooseField->setVisible(true);
            ui->inputField->setVisible(false);
            ui->inputField->setText("");
            ui->inputField->setStyleSheet("border: none;");

            setDisabledField();
        }
    }
}

void CourtCaseWindow::clickedCloseEditTemplate() {
    ui->chooseField->setCurrentIndex(0);
    ui->mainWidget->setVisible(true);
    ui->editWidget->setVisible(false);
}

void CourtCaseWindow::clickedConfirmEdit() {
    QPushButton *currentButton = (QPushButton*)this->sender();

    QJsonObject jsonObject;
    QJsonArray templateFields;
    for (auto item : templateFieldSet) {
        templateFields.append(item);
    }
    jsonObject.insert("fields_array", templateFields);

    if (currentButton == ui->editTemplate) {
        db->updateJudgment(
                    ui->chooseTemplate->currentText(),
                    QJsonDocument::fromVariant(jsonObject.toVariantMap()),
                    ui->textEdit->toPlainText()
                    );
        createFieldPage(
                    QJsonArray(db->getFieldsForJudgment(ui->chooseTemplate->currentText()))
                    );
    } else {
        db->addJudgment(
                    ui->nameTemplate->text(),
                    QJsonDocument::fromVariant(jsonObject.toVariantMap()),
                    ui->textEdit->toPlainText(),
                    (ui->legalResponsibility->isChecked()) ? true : false
                    );
        ui->chooseTemplate->addItem(
                    ui->nameTemplate->text()
                    );
        ui->nameTemplate->setText("");
        ui->chooseField->setCurrentIndex(0);
        ui->textEdit->setText("");
    }
    ui->mainWidget->setVisible(true);
    ui->editWidget->setVisible(false);
}

void CourtCaseWindow::clickedDeleteField() {
    ui->textEdit->setText(
                QString(ui->textEdit->toPlainText()).remove(allFields.key(ui->chooseField->currentText())));
    templateFieldSet.remove(
                allFields.key(ui->chooseField->currentText())
                );
}

void CourtCaseWindow::insertFieldInTemplate() {
    ui->textEdit->insertPlainText(
                allFields.key(ui->chooseField->currentText())
                );
    templateFieldSet.insert(
                allFields.key(ui->chooseField->currentText())
                );
}

void CourtCaseWindow::changeMainPageStyle() {
    if (ui->stackedWidget->currentIndex() == 0) {
        ui->mainWidget->setStyleSheet(StyleHelper::getFirstPageStyle());
        ui->mainWidget->setMinimumSize(550, 401);
        ui->mainWidget->setMaximumSize(550, 401);
    } else {
        ui->mainWidget->setStyleSheet(StyleHelper::getSecondPageStyle());
        ui->firstPageButton->setStyleSheet(StyleHelper::getFirstPageButtonStyle());
        ui->secondPageButton->setStyleSheet(StyleHelper::getSecondPageButtonStyle());
        ui->secondPageButton->setMinimumSize(53, 53);
        ui->mainWidget->setMinimumSize(550, 614);
        ui->mainWidget->setMaximumSize(550, 614);
    }
}

void CourtCaseWindow::clickedFirstPage() {
    ui->stackedWidget->setCurrentIndex(0);
    changeMainPageStyle();
}

void CourtCaseWindow::clickedSecondPage() {
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
                ui->fieldStackedWidget->setCurrentIndex(firstEmptyIndex/6);
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
    QString filename(ui->filenameEdit->text() + " " + ui->chooseTemplate->currentText());
    QFile file(pathToLastFileDir + filename + ".docx");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream fileout(&file);
        fileout << text;
        file.close();
    }
}

void CourtCaseWindow::createCourtCase() {
    if (!checkFillingFields()) {
        QString text = db->getTextTemplate(ui->chooseTemplate->currentText());
        for (auto item: fieldsList) {
            text.replace(allFields.key(item->placeholderText()), item->text());
        }
        createFile(text);
        this->close();
    }
}

void CourtCaseWindow::clickedConfirmButton() {
    if (ui->stackedWidget->currentIndex() == 0 &&
        ui->chooseTemplate->currentText() != "" &&
        ui->filenameEdit->text() != "") {
        ui->secondPageButton->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(1);
        changeMainPageStyle();
    } else if (ui->stackedWidget->currentIndex() == 1) {
        createCourtCase();
    }
}

void CourtCaseWindow::clearFieldStackedWidget() {
    for(int i = ui->fieldStackedWidget->count(); i >= 0; i--) {
        QWidget* widget = ui->fieldStackedWidget->widget(i);
        ui->fieldStackedWidget->removeWidget(widget);
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
    connect(slideLeftButton, SIGNAL(clicked()), this, SLOT(clickedSlideButton()));
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
        connect(pageButton, SIGNAL(clicked()), this, SLOT(clickedPageButton()));
    }

    QPushButton *slideRightButton = new QPushButton();
    slideRightButton->setStyleSheet(StyleHelper::getRightSlideStyle());
    slideRightButton->setMinimumSize(12, 12);
    slideRightButton->setMaximumSize(12, 12);
    ui->buttonLayout->addWidget(slideRightButton);
    pageButtonList.append(slideRightButton);
    (buttonsCount > 4) ? slideRightButton->setVisible(true) : slideRightButton->setVisible(false);
    connect(slideRightButton, SIGNAL(clicked()), this, SLOT(clickedSlideButton()));

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

void CourtCaseWindow::createFieldPage(QJsonArray fieldsArray) {
    clearFieldStackedWidget();
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
                            allFields.value(fieldsArray.first().toString())
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
        ui->fieldStackedWidget->addWidget(pageWidget);
        pageWidget->setLayout(pageLayout);
    }
}

void CourtCaseWindow::changeComboBox(int index) {
    QString caseName = ui->chooseTemplate->itemText(index);

    ui->chooseTemplate->setStyleSheet(StyleHelper::getComboboxWithoutBorderStyle());
    if (index != 0) {
        createFieldPage(QJsonArray(db->getFieldsForJudgment(caseName)));
    }
}

void CourtCaseWindow::clickedPageButton() {
    for (int i = 1; i < pageButtonList.size() - 1; i++) {
        pageButtonList[i]->setStyleSheet(
                    "image: url(:/courtcasewindow/images/courtcasewindow/page-button.png);"
                    "border:none;");
    }
    QPushButton *currentButton = (QPushButton*)this->sender();
    ui->fieldStackedWidget->setCurrentIndex(pageButtonList.indexOf(currentButton) - 1);
    currentButton->setStyleSheet(
                "image: url(:/courtcasewindow/images/courtcasewindow/page-button-switch.png);"
                "border:none;");
}

void CourtCaseWindow::clickedSlideButton() {
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
