#ifndef COURTCASEWINDOW_H
#define COURTCASEWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>

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
    void secondPageClicked();
    void firstPageClicked();
    void changeComboBox(int index);
    void pageButtonClicked();
    void slideButtonClicked();
    void okeyButtonClicked();

private:
    Ui::CourtCaseWindow *ui;
    DataBase *db;

    void changeMainPageStyle();

    void makePageButtons(int buttonsCount);
    void clearButtonsLayout();
    void clearStackedWidget();
    QList<QPushButton*> pageButtonList;
    int buttonDifference = 0;

    void createCourtCase();
    bool checkFillingFields();
    QList<QLineEdit*> fieldsList;

    QMap<QString, QString> allFields;
    void createFile(QString text);
};

#endif // COURTCASEWINDOW_H
