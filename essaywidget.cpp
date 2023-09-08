#include "essaywidget.h"

essayWidget::essayWidget(QWidget *parent) : QWidget(parent)
{
    this->layout();
    this->behavior();
}

void essayWidget::layout() {
    this->resize(600,400);
    this->setWindowTitle("随笔");
    this->setWindowIcon(QIcon(":imgs/侦查.png"));
    this->basePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).append("/default.txt");
    QGridLayout * container = new QGridLayout(this);
    QGridLayout * tab = new QGridLayout();
    QHBoxLayout * pathLayout = new QHBoxLayout();
    tab->addLayout(pathLayout,1,0,1,5);
    QGridLayout * main = new QGridLayout();
    container->addLayout(tab,0,0);
    container->addLayout(main,1,0);

    QFont font_btn("幼圆",10);
    btn_open = new QPushButton("打开文件",this);
    btn_open->setFont(font_btn);
    btn_save = new QPushButton("保存",this);
    btn_save->setFont(font_btn);
    btn_save_other = new QPushButton("另存为",this);
    btn_save_other->setFont(font_btn);
    btn_close = new QPushButton("关闭文件",this);
    btn_close->setFont(font_btn);
    QFont font_line("方正舒体",12);
    lineEdit = new QLineEdit(this);
    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lineEdit->setFont(font_line);
    lineEdit->setFixedHeight(30);
    lineEdit->setText(this->basePath);
    lineEdit->setReadOnly(true);
    path = new QLabel(this);
    path->setText("filepath:");
    path->setFixedHeight(30);
    path->setFixedWidth(110);
    path->setFont(font_line);
    path->setAlignment(Qt::AlignRight);
    tab->addWidget(btn_open,0,0);
    tab->addWidget(btn_save,0,1);
    tab->addWidget(btn_save_other,0,2);
    tab->addWidget(btn_close,0,4);
    pathLayout->addWidget(path);
    pathLayout->addWidget(lineEdit);
    text_area = new QTextEdit(this);
    QFont font_text("方正舒体",16);
    text_area->setFont(font_text);
    text_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    main->addWidget(text_area,0,0);

    this->setAttribute(Qt::WA_QuitOnClose, false);
}

void essayWidget::behavior() {
    connect(btn_open,&QPushButton::clicked,this,&essayWidget::OpenFile);
    connect(btn_save,&QPushButton::clicked,this,&essayWidget::SaveFile);
    connect(btn_save_other,&QPushButton::clicked,this,&essayWidget::saveOtherFile);
    connect(btn_close,&QPushButton::clicked,this,&essayWidget::CloseFile);
    this->initFile();
}

void essayWidget::initFile() {
    QFile file_undetd(this->basePath);
    if(!file_undetd.exists()) {
        if (file_undetd.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QTextStream outputStream(&file_undetd);
            outputStream.setCodec("Latin-1");  // 设置编码为Latin-1
            outputStream << "好好考研\n";  // 写入内容
            outputStream << "此文件默认保存在" << this->basePath;
            file_undetd.close();
        }
    }

    QFile file(this->basePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QByteArray data = file.readAll();   /* 读取文件对象中的所有数据 */
    text_area->setText(data);
    file.close();  // 关闭文件
}

void essayWidget::OpenFile() {
    /* 文件对话框，选择需要打开的文件 */
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      ".",
                                                      tr("Text files (*.txt)"));
    lineEdit->setText(fileName);        /* 需要打开文件的路径名称在lineEdit行编辑框中显示 */
    QFile file(fileName);           /* 构造文件类对象 */
    /* 以只读方式打开文本文件对象 */
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    this->basePath = fileName;

    QTextStream inputStream(&file);
    inputStream.setCodec("UTF-8");  // 设置文件编码，替换为实际的文件编码
    QString content = inputStream.readAll();
    text_area->setText(content);
    file.close();  // 关闭文件
}

void essayWidget::SaveFile() {
    if(this->lineEdit->text().isEmpty())
        this->saveOtherFile();
    else {
        QFile file(this->lineEdit->text());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out.setCodec("UTF-8");  // 设置编码为Latin-1
        out << text_area->toPlainText();
        file.close();
    }
}

void essayWidget::saveOtherFile() {
    QString fileName;
    /* 使用保存文件对话框，选择内容保存文件的文件名称 */
    fileName = QFileDialog::getSaveFileName(this, tr("另存为"),".",tr("Text files (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out.setCodec("UTF-8");  // 设置编码为Latin-1
    out << text_area->toPlainText();
    file.close();
}

void essayWidget::CloseFile() {
    text_area->clear();
    lineEdit->clear();
}
