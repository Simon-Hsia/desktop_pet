#ifndef TODOWIDGET_H
#define TODOWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QIcon>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
class todoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit todoWidget(QWidget *parent = nullptr);

signals:

public slots:
    void addTodoItem();
    void moveToDoneList(QListWidgetItem *);
    void moveAllToDoneList();
    void delAllDoneList();
    void saveList(bool);
private:
    QLineEdit *lineEdit;
    QPushButton *btn_add;
    QListWidget *todoList;
    QListWidget *doneList;
    int total_todo;
    QLineEdit *total_todo_line;
    QPushButton *del_all_todo;
    int total_done;
    QLineEdit *total_done_line;
    QPushButton *del_all_done;

    void initData();
    void initLayout();
    void behavior();
    void setTotal();
    void loadList(bool flag);
    void closeEvent(QCloseEvent *event);
};

#endif // TODOWIDGET_H
