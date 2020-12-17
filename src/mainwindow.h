#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QToolButton>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QDateTime>
#include <QMenu>
#include <QDialog>

#include <src/database/database.h>
#include <src/stylehelper/stylehelper.h>
#include <src/courtcasewindow/courtcasewindow.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPoint previousPosition() const;

public slots:
    void setPreviousPosition(QPoint previousPosition);

private slots:
    void pageButtonClicked();
    void sortActionTriggired();
    void addButtonClicked();

signals:
    void previousPositionChanged(QPoint previousPosition);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject * obj, QEvent *event);

private:
    Ui::MainWindow *ui;
    DataBase *db = new DataBase();

    // For change and resize window
    QPoint mousePreviousPosition;
    bool isMoveWindow = false;

    // Lates templates
    QList<QPushButton*> pageButtonList;
    int templateCount = 0;
    int buttonDifference = 0;
    void addPageButtons(int buttonsCount);
    void clearStackedWidget();
    void addLatestTemplates();
    void createLatestTemplate(QJsonObject json);

    // Sorted latest template
    void addMenuForSort();
    CourtCaseWindow *courtCaseWindow;

};
#endif // MAINWINDOW_H
