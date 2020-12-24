#ifndef COURTCASEWINDOW_H
#define COURTCASEWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QSet>

#include <src/database/database.h>
#include <src/stylehelper/stylehelper.h>

namespace Ui {
    class CourtCaseWindow;
}

class CourtCaseWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CourtCaseWindow(DataBase *dbMainWindow, QWidget *parent = nullptr);
    ~CourtCaseWindow();

private slots:
    void clickedSecondPage();
    void clickedFirstPage();
    void changeComboBox(int index);
    void clickedPageButton();
    void clickedSlideButton();
    void clickedConfirmButton();

    void clickedDeleteTemplate();

    void showEditTemplatePage();
    void insertFieldInTemplate();
    void clickedConfirmEditButton();
    void clickedDeleteFieldButton();

private:
    Ui::CourtCaseWindow *ui;
    DataBase *db;

    void changeMainPageStyle();

    void createFieldPage(QJsonArray fieldsArray);
    void makePageButtons(int buttonsCount);
    void clearButtonsLayout();
    void clearFieldStackedWidget();
    QList<QPushButton*> pageButtonList;
    int buttonDifference = 0;

    void createCourtCase();
    bool checkFillingFields();
    QList<QLineEdit*> fieldsList;

    QMap<QString, QString> allFields;
    void createFile(QString text);

    QSet<QString> templateFieldSet;
};

#endif // COURTCASEWINDOW_H
