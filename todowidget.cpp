#include "todowidget.h"

todoWidget::todoWidget(QWidget *parent) : QWidget(parent)
{
    this->initData();
    this->initLayout();
    this->behavior();

    // 在构造函数中加载待办清单
    this->loadList(true);
    this->loadList(false);
}

//  初始化数据
void todoWidget::initData() {
    this->total_todo = 0;
    this->total_done = 0;
    this->setAttribute(Qt::WA_QuitOnClose, false);
}

//  初始化布局
void todoWidget::initLayout() {
    this->setWindowTitle("待办清单");
    this->setWindowIcon(QIcon(":imgs/好奇.png"));
    QGridLayout *mainLayout = new QGridLayout(this);

    QHBoxLayout *addLayout = new QHBoxLayout();
    this->lineEdit = new QLineEdit(this);
    this->btn_add = new QPushButton("添加待办",this);
    this->btn_add->setFixedSize(100,30);
    addLayout->addWidget(this->lineEdit);
    addLayout->addWidget(this->btn_add);
    mainLayout->addLayout(addLayout,0,0);

    this->todoList = new QListWidget(this);
    mainLayout->addWidget(this->todoList,1,0);

    this->total_todo_line = new QLineEdit(this);
    this->total_todo_line->setStyleSheet("background-color: transparent; border: none;");
    this->del_all_todo = new QPushButton("全部完成");
    QHBoxLayout *allTodoLayout = new QHBoxLayout();
    allTodoLayout->addWidget(new QLabel("总数:"));
    allTodoLayout->addWidget(this->total_todo_line);
    allTodoLayout->addWidget(this->del_all_todo);
    mainLayout->addLayout(allTodoLayout,2,0);

    this->doneList = new QListWidget(this);
    mainLayout->addWidget(this->doneList,3,0);

    this->total_done_line = new QLineEdit(this);
    this->total_done_line->setStyleSheet("background-color: transparent; border: none;");
    this->del_all_done = new QPushButton("清除全已办");
    QHBoxLayout *allDoneLayout = new QHBoxLayout();
    allDoneLayout->addWidget(new QLabel("总数:"));
    allDoneLayout->addWidget(this->total_done_line);
    allDoneLayout->addWidget(this->del_all_done);
    mainLayout->addLayout(allDoneLayout,4,0);
}

//  绑定行为
void todoWidget::behavior() {
    connect(this->btn_add,&QPushButton::clicked,this,&todoWidget::addTodoItem);
    connect(this->del_all_todo,&QPushButton::clicked,this,&todoWidget::moveAllToDoneList);
    connect(this->del_all_done,&QPushButton::clicked,this,&todoWidget::delAllDoneList);
    connect(this->todoList, &QListWidget::itemDoubleClicked, this,  [=](QListWidgetItem *item) {
        this->moveToDoneList(item);
    });
    this->setTotal();
}

// 添加列表项
void todoWidget::addTodoItem() {
    if(this->lineEdit->text().isEmpty())
        this->lineEdit->setText("不能为空");
    else {
        QListWidgetItem* item = new QListWidgetItem("     " + this->lineEdit->text());
        QCheckBox* checkBox = new QCheckBox();
        checkBox->setCheckState(Qt::Unchecked); // 默认为未勾选状态
        // 连接复选框的 stateChanged 信号到一个槽函数,实现完成item操作
        connect(checkBox, &QCheckBox::stateChanged, this,  [=]() {
            this->moveToDoneList(item);
        });
        // 添加 QListWidgetItem 到 QListWidget
        this->todoList->addItem(item);
        // 将复选框设置为 QListWidgetItem 的小部件
        this->todoList->setItemWidget(item, checkBox);
        this->total_todo++;
        this->setTotal();
        this->lineEdit->setText("");
    }
}

//转移到已办项
void todoWidget::moveToDoneList(QListWidgetItem* item) {
    // 获取当前选中的行索引
    int row = this->todoList->row(item);
    // 移除选中的行
    QListWidgetItem* movetoItem = this->todoList->takeItem(row);
    this->doneList->addItem(movetoItem);
    this->total_todo--;
    this->total_done++;
    this->setTotal();
}

//转移所有到已办项
void todoWidget::moveAllToDoneList() {
    int itemCount = this->todoList->count(); // 获取列表中的项数
    for (int i = 0; i < itemCount; i++) {
        QListWidgetItem* item = this->todoList->item(0); // 获取到第一个，因为不断的在删除，i没用
        this->moveToDoneList(item);
    }
}

//  删除已完成清单所有项
void todoWidget::delAllDoneList() {
    int itemCount = this->doneList->count(); // 获取列表中的项数
    for (int i = 0; i < itemCount; i++) {
        QListWidgetItem* item = this->doneList->item(0);
        this->doneList->takeItem(0);    // 移除选中的行
        delete item;
    }
    this->total_done = 0;
    this->setTotal();
}

//设置总数
void todoWidget::setTotal() {
    this->total_todo_line->setText(QString::number(this->total_todo));
    this->total_done_line->setText(QString::number(this->total_done));
}

// 保存待办清单到文件
void todoWidget::saveList(bool flag)
{
    QJsonArray array;
    QListWidget *list = flag?this->todoList:this->doneList;

    for (int i = 0; i < list->count(); i++) {
        QListWidgetItem* item = list->item(i);
        QJsonObject object;
        object.insert("content", item->text());
        array.append(object);
    }

    QJsonDocument doc(array);
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if(flag)
        path = path.append("/todolist.json");
    else
        path = path.append("/donelist.json");
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    } else {
        QFile newFile(path);
        if (newFile.open(QIODevice::WriteOnly)) {
            newFile.write(doc.toJson());
            newFile.close();
        }
    }
}

// 从文件中加载待办清单和已完成清单
void todoWidget::loadList(bool flag)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if(flag)
        path = path.append("/todolist.json");
    else
        path = path.append("/donelist.json");
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            QJsonArray doneArray = doc.array();
            for (int i = 0; i < doneArray.size(); i++) {
                QJsonValue itemValue = doneArray.at(i);
                if (itemValue.isObject()) {
                    QJsonObject itemObject = itemValue.toObject();
                    QString content = itemObject.value("content").toString();
                    QListWidgetItem* item = new QListWidgetItem(content);
                    if(flag) {
                        QCheckBox* checkBox = new QCheckBox();
                        checkBox->setCheckState(Qt::Unchecked); // 默认为未勾选状态
                        // 连接复选框的 stateChanged 信号到一个槽函数,实现完成item操作
                        connect(checkBox, &QCheckBox::stateChanged, this,  [=]() {
                            this->moveToDoneList(item);
                        });
                        // 添加 QListWidgetItem 到 QListWidget
                        this->todoList->addItem(item);
                        // 将复选框设置为 QListWidgetItem 的小部件
                        this->todoList->setItemWidget(item, checkBox);
                        this->total_todo++;
                    }else {
                        this->doneList->addItem(item);
                        this->total_done++;
                    }
                }
            }
            this->setTotal();
        }
    }
}

void todoWidget::closeEvent(QCloseEvent *event) {
    // 进行清理操作，例如保存数据等
    this->saveList(true);
    this->saveList(false);
}

