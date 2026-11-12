#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    this->wid_login = new loginWidget;
    this->setWindowTitle("监控系统主页面");
    this->setMinimumSize(1600,1000);
    resize(1600,1000);

    //窗口主布局采集垂直布局
    QVBoxLayout* main_layout = new QVBoxLayout;
    this->setLayout(main_layout);

    //顶部导航栏布局
    top_layout = new QHBoxLayout;
    //下方窗口

    //*******
    QStackedLayout *buttom_layout = new QStackedLayout;
    //*******

    MonitorWidget* m_widget = new MonitorWidget;
    buttom_layout->addWidget(m_widget);

    //将上面两个布局都加入到主布局容器中
    main_layout->addLayout(top_layout,1);
    main_layout->addLayout(buttom_layout,8);

    //初始化顶部导航窗口
    this->init_top_widget();
    this->init_qss();
    this->init_connect();
    this->init_showtime();
    this->show();
}

/*
函数名：init_showtime
函数参数：无
函数功能：
1. 创建定时器，间隔 1000ms
2. 绑定信号槽：每秒触发`update_time()
3. start()定时器跑起来
4. 立刻手动跑一遍 update_time () → 界面马上显示时间
5. 之后每过 1 秒，timeout 信号触发，自动跑 update_time，时间数字跳动
*/
void MainWidget::init_showtime()
{
    m_timer = new QTimer(this);  //当初创建的是局部变量

    //设置定时器间隔时间为1秒
    m_timer->setInterval(1000);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(update_time()));

    //启动定时器
    m_timer->start();

    update_time();
}

/*
函数名：
函数参数：
函数功能：
*/
void MainWidget::init_top_widget()
{
    QLabel *lab_title = new QLabel("安防监控系统管理平台");
    lab_title->setObjectName("title");
    this->top_layout->addWidget(lab_title);
    this->top_layout->addStretch();

    QPushButton *btn_jiankong = new QPushButton(QIcon(":/imge/jiankong.png"),"监控");
    this->top_layout->addWidget(btn_jiankong);

    QPushButton *btn_review = new QPushButton(QIcon(":/imge/huifang.png"),"回放");
    this->top_layout->addWidget(btn_review);

    QPushButton *btn_setting = new QPushButton(QIcon(":/imge/seting.png"),"设置");
    this->top_layout->addWidget(btn_setting);

    datepicker = new Datepicker;
    top_layout->addWidget(datepicker);

    lab_time = new QLabel("");
    top_layout->addWidget(lab_time);
    lab_time->setObjectName("ctime");

    QWidget *btn_login_box = new QWidget;
    QVBoxLayout* btn_login_layout = new QVBoxLayout(btn_login_box);

    btn_login = new QPushButton("");

    btn_login->setIcon(QIcon(":/imge/user.png"));
    btn_login->setIconSize(QSize(40,50));
    btn_login_layout->addWidget(btn_login);

    lab_login = new QLabel("未登录");

    lab_login->setAlignment(Qt::AlignCenter);

    btn_login_layout->addWidget(lab_login);
    btn_login_layout->addStretch();
    top_layout->addWidget(btn_login_box);
}

/*
函数名：
函数参数：
函数功能：
*/
void MainWidget::init_qss()
{
    QFile file(":/qss/mian.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}


/*
函数名：
函数参数：
函数功能：
*/
void MainWidget::update_time()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("yyyy-MM-dd HH:mm:ss");
    lab_time->setText(timeString);
}

/*
函数名：
函数参数：
函数功能：
*/
void MainWidget::init_connect()
{
    connect(btn_login, SIGNAL(clicked()), this, SLOT(to_login_win()));
    connect(wid_login, SIGNAL(login_success(QString)), this , SLOT(back_to_mainWin(QString)));
}

/*
函数名：
函数参数：
函数功能：
*/
void MainWidget::to_login_win()
{
    this->wid_login->show();
}

/*
函数名：
函数参数：
函数功能：
*/
void MainWidget::back_to_mainWin(QString username)   //只绑定一次，之后无数次点击按钮，都会自动触发槽，不需要反复调用init_connect()
{
    this->wid_login->hide();
    lab_login->setText(username);
    this->show();
}













