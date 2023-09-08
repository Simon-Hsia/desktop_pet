#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->initData();
    this->initLayout();
    //  开始随机时长随机切换动图
    this->cut_timer.start();
}

//  初始化全局数据
void Widget::initData() {

    this->initWdigetShowData();
    this->initImgsData();
    this->initFlyData();
    this->initMenuData();
    this->initTrayData();

}

//  初始化窗体显示相关数据
void Widget::initWdigetShowData() {
    this->cut_interval = this->getRandom(5, 121) * 1000;
    this->cut_timer.setInterval(this->cut_interval);
    connect(&this->cut_timer,SIGNAL(timeout()),this,SLOT(handleCutTimer()));
    this->window_width = 240;
    this->window_height = 240;
    // 获取桌面窗口
    QScreen  *screen = QGuiApplication::primaryScreen();
    // 获取主屏幕的大小
    QRect screenGeometry = screen->availableGeometry();;
    this->screen_width = screenGeometry.width();
    this->screen_height = screenGeometry.height();
    // 设置窗体点击关闭整个应用程序不会关闭
    this->setAttribute(Qt::WA_QuitOnClose, false);
}

//  初始化图片相关数据
void Widget::initImgsData() {
    this->imgs.push_back(":imgs/做鬼脸.gif");
    this->imgs.push_back(":imgs/运动.gif");
    this->imgs.push_back(":imgs/煎蛋.gif");
    this->imgs.push_back(":imgs/难过.gif");
    this->imgs.push_back(":imgs/铜锣烧.gif");
    this->imgs.push_back(":imgs/神笔.gif");
    this->imgs.push_back(":imgs/观望.gif");
    this->imgs.push_back(":imgs/点头.gif");
    this->imgs.push_back(":imgs/生气.gif");
    this->imgs.push_back(":imgs/生日.gif");
    this->imgs.push_back(":imgs/飞行.gif");

    this->label = new QLabel(this);//  创建一个label存放动图
    this->current_index = this->getRandom(this->imgs.size() - 1);
    this->movie = new QMovie(this);
    this->movie->setFileName(this->imgs.at(this->current_index));
    //  设置QMovie的缩放大小，以适应窗体的大小
    this->movie->setScaledSize(QSize(this->window_width, this->window_height));
}

//  初始化飞行相关数据
void Widget::initFlyData() {
    this->fall_interval = 20;
    this->drift_scope = 600;
    this->drift_distance = this->drift_scope;
    this->step_x_number = 20;
    this->step_x = this->drift_distance / this->step_x_number;
    this->step_x_sign = 1;
    this->fly_flag = false;

    this->fall_timer.setInterval(this->fall_interval);
    connect(&this->fall_timer,SIGNAL(timeout()),this,SLOT(handleFallTimer()));
    this->drift_timer.setInterval(this->fall_interval);
    connect(&this->drift_timer,SIGNAL(timeout()),this,SLOT(handleDriftTimer()));
}

//  初始化菜单数据
void Widget::initMenuData() {
    this->menu = new QMenu(this); // 创建右键菜单对象
    QAction *action_todo = new QAction(tr("  待办"));
    QAction *action_essay = new QAction(tr("  随笔"));
    QAction *action_draw = new QAction(tr("  随想"));
    QAction *action_mouch = new QAction(tr("  鬼脸"));
    QAction *action_exercise = new QAction(tr("  运动"));
    QAction *action_next = new QAction(tr("下一张"));
    QAction *action_min = new QAction(tr("最小化"));
    this->menu->addAction(action_todo);
    action_todo->setIcon(QIcon(":imgs/好奇.png"));
    this->menu->addAction(action_essay);
    action_essay->setIcon(QIcon(":imgs/侦查.png"));
    this->menu->addAction(action_draw);
    action_draw->setIcon(QIcon(":imgs/思考.png"));
    this->menu->addAction(action_mouch);
    action_mouch->setIcon(QIcon(":imgs/做鬼脸.gif"));
    this->menu->addAction(action_exercise);
    action_exercise->setIcon(QIcon(":imgs/运动.gif"));
    this->menu->addAction(action_next);
    action_min->setIcon(QIcon(":imgs/匆忙.png"));
    this->menu->addAction(action_min);

    this->todo_widget = new todoWidget;
    this->essay_widget = new essayWidget;

    // 连接菜单项的触发信号和槽函数
    connect(action_todo, &QAction::triggered, this->todo_widget, &todoWidget::show);
    connect(action_essay, &QAction::triggered, this->essay_widget, &essayWidget::show);
    connect(action_draw, &QAction::triggered, this, &Widget::openPaint);
    connect(action_mouch, &QAction::triggered, this, &Widget::makeMouch);
    connect(action_exercise, &QAction::triggered, this, &Widget::exercise);
    connect(action_next, &QAction::triggered, this, &Widget::next);
    connect(action_min, &QAction::triggered, this, &Widget::minimality);
}

//  初始化托盘数据
void Widget::initTrayData() {
    this->trayIcon = new QSystemTrayIcon(this); // 创建系统托盘对象
    this->trayIcon->setIcon(QIcon(":imgs/匆忙.png")); // 设置托盘图标

    QMenu* menu = new QMenu(this);  // 创建右键菜单
    QAction* exitAction = new QAction("退出", this);// 创建退出项
    connect(exitAction, &QAction::triggered, QApplication::instance(), &QApplication::quit);

    menu->addAction(exitAction);

    // 将菜单设置为系统托盘图标的右键菜单
    trayIcon->setContextMenu(menu);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Widget::trayIconActivated);
}

//  初始化结构布局
void Widget::initLayout() {
    this->setFixedSize(this->window_width,this->window_height);
    this->move((this->screen_width - this->window_width),(this->screen_height - this->window_height));
    //  隐藏外部窗口，隐藏任务栏图标，背景透明，这四句话缺一不可
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setWindowFlag(Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground,true);

    this->changeMovie(this->current_index);
    label->setMovie(this->movie);
    label->setScaledContents(true); //  图像自适应label长宽

    QString menuStyle = "QMenu {"
                            "border: 2px solid black;"
                        "}"
                        "QMenu::item {"
                            "background-color: white;"
                            "padding: 5px 20px 5px 20px;" // 设置菜单项的内边距
                        "}"
                        "QMenu::item:selected {"
                            "background-color: #00a1e9;" // 设置菜单项选中时的背景颜色
                        "}";
    this->menu->setStyleSheet(menuStyle);
}

//  重写鼠标移动事件
void Widget::mouseMoveEvent(QMouseEvent *event) {
    if(this->current_btn != Qt::RightButton) {
        int move_x = event->x() + this->pos().x() - this->inline_x;
        int move_y = event->y() + this->pos().y() - this->inline_y;
        this->move(move_x,move_y);
    }
}

//  重写鼠标按下事件
void Widget::mousePressEvent(QMouseEvent *event) {
    this->current_btn = event->button();
    if(this->current_btn == Qt::LeftButton) {
        if(this->fly_flag) {
            this->fall_timer.stop();
            this->drift_timer.stop();
            this->fly_flag = false;
            this->changeMovie(this->current_index);
        }
        this->inline_x = event->x();
        this->inline_y = event->y();
    }
}

//  重写鼠标按键弹起事件
void Widget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton && this->fly_flag) {
        this->fall_timer.start();
        this->drift_timer.start();
    }
    else if(event->button() == Qt::RightButton)
        this->menu->exec(event->globalPos());// 在鼠标右键位置显示右键菜单
}

//  双击飞行
void Widget::mouseDoubleClickEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        this->changeMovie(this->imgs.size() - 1);//    飞行动图
        if(this->fly_flag) {
            this->fall_timer.stop();
            this->drift_timer.stop();
        }
        this->fly_flag = true;
    }
}

// 右键菜单打开随想画图槽函数
void Widget::openPaint() {
     QProcess::startDetached("mspaint");
}

//  做鬼脸选项槽函数
void Widget::makeMouch() {
    this->changeMovie(0);
    this->restartCutTimer();
}

//  运动选项槽函数
void Widget::exercise() {
    this->changeMovie(1);
    this->restartCutTimer();
}

//  切换下一张槽函数
void Widget::next() {
    if(++this->current_index >= this->imgs.size() - 1)
        this->current_index = 0;
    this->changeMovie(this->current_index);
    this->restartCutTimer();
}

//  最小化到托盘槽函数
void Widget::minimality() {
    this->hide(); // 隐藏主窗口
    this->trayIcon->show(); // 显示托盘图标
}

// 恢复应用程序窗口槽函数
void Widget::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        // 点击托盘图标时恢复窗口
        this->showNormal(); // 恢复窗口
        trayIcon->hide(); // 隐藏托盘图标
    }
}

//  随机切换动图定时器槽函数
void Widget::handleCutTimer() {
    this->current_index = this->getRandom(5);
    if(!this->fly_flag)
    this->changeMovie(this->current_index);
    this->cut_interval = this->getRandom(5, 121) * 1000;
    this->cut_timer.setInterval(this->cut_interval);
}

//  下落定时器的槽函数
void Widget::handleFallTimer() {
    if(this->pos().y() + this->window_height < this->screen_height)
        this->move(this->pos().x(),this->pos().y() + 2);
    else {
        this->fall_timer.stop();
        this->drift_timer.stop();
        this->fly_flag = false;
        this->changeMovie(this->current_index);
        this->restartCutTimer();
    }
}

//  树叶飘动效果定时器槽函数
void Widget::handleDriftTimer() {

    this->move(this->pos().x() + (this->step_x * this->step_x_sign),this->pos().y());
    this->drift_distance -= this->step_x;
    this->step_x = this->drift_distance / this->step_x_number;

    if(this->step_x <= 1 ||
      (this->pos().x() + this->window_width) >= this->screen_width ||
      this->pos().x() <= 0) {
        this->step_x_sign = -this->step_x_sign;
        this->drift_distance = this->drift_scope;
    }
}

//  工具函数：重新启动切换动图定时器
void Widget::restartCutTimer() {
    this->cut_timer.stop();
    this->cut_interval = this->getRandom(5, 11) * 1000;
    this->cut_timer.setInterval(this->cut_interval);
    this->cut_timer.start();
}

//  工具函数：切换动图
void Widget::changeMovie(int index) {
    this->movie->stop();
    this->movie->setFileName(this->imgs.at(index));
    //  循环播放信号槽
    movie->start();
}

//  工具函数：获取一定范围内随机数
int Widget::getRandom(int border_1,int border_2) {
    if(border_2)
        return QRandomGenerator::global()->bounded(border_1,border_2);
    else
        return QRandomGenerator::global()->bounded(border_1);
}

Widget::~Widget()
{

}
