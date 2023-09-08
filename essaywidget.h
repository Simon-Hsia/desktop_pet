#ifndef ESSAYWIDGET_H
#define ESSAYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QFont>
#include <QString>
#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QTextCodec>
#include <QIcon>
#include <QStandardPaths>

class essayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit essayWidget(QWidget *parent = nullptr);

signals:

private slots:
    void OpenFile();
    void SaveFile();
    void saveOtherFile();

public:
    Widget(QWidget *parent = 0);

    QPushButton* btn_open;
    QPushButton* btn_save;
    QPushButton* btn_save_other;
    QPushButton* btn_close;
    QLineEdit *lineEdit;
    QString basePath;
    QLabel *path;
    QTextEdit* text_area;
    void layout();
    void behavior();
    void setFont();
    void setColor();
    void CloseFile();
    void initFile();
};

#endif // ESSAYWIDGET_H
