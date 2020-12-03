#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>

#include <src/database/database.h>
#include <src/stylehelper/stylehelper.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPoint previousPosition() const;

public slots:
    void setPreviousPosition(QPoint previousPosition);

private slots:
    void pageButtonClicked();

signals:
    void previousPositionChanged(QPoint previousPosition);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    DataBase *db = new DataBase();

    // For change and resize window
    QPoint mousePreviousPosition;
    bool isMoveWindow = false;

    // Add latest templates on main window
    QList<QPushButton*> pageButtonList;
    void addPageButtons(int buttonsCount);
    QList<QWidget*> pageList;
    void cleanStackedWidget();
    void addLatestTemplates();
};
#endif // MAINWINDOW_H
