#ifndef COURTCASEWINDOW_H
#define COURTCASEWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <src/database/database.h>

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

    void addPageButtons(int buttonsCount);
    void clearButtonsLayout();
    void clearStackedWidget();
    QList<QPushButton*> pageButtonList;
    int buttonDifference = 0;
};

#endif // COURTCASEWINDOW_H
