#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QMouseEvent>
#include <QString>
#include <QVector>
#include <QRandomGenerator>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QTimer>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include <todowidget.h>
#include <essaywidget.h>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QApplication>
class Widget : public QWidget
{
    Q_OBJECT

private slots:
    void handleFallTimer();
    void handleDriftTimer();
    void handleCutTimer();
    void makeMouch();
    void exercise();
    void openPaint();
    void minimality();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
private:
    QSystemTrayIcon *trayIcon;
    QLabel *label;
    QMovie *movie;
    QVector<QString> imgs;
    QMenu *menu;
    enum Qt::MouseButton current_btn;
    int inline_x;
    int inline_y;
    int window_width;
    int window_height;
    int screen_width;
    int screen_height;
    int fall_interval;
    int cut_interval;
    int step_x;
    int step_x_sign;
    int step_x_number;
    int drift_scope;
    int drift_distance;
    QTimer fall_timer;
    QTimer drift_timer;
    QTimer cut_timer;
    int current_index;
    bool fly_flag;

    void initData();
    void initImgsData();
    void initFlyData();
    void initMenuData();
    void initTrayData();
    void initWdigetShowData();
    void initLayout();
    void changeMovie(int);
    int getRandom(int border_1,int border_2 = 0);
    void restartCutTimer();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void menuAction1();
    void menuAction2();
    void mouseDoubleClickEvent(QMouseEvent *event);
    void next();

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    todoWidget *todo_widget;
    essayWidget *essay_widget;
};

#endif // WIDGET_H
