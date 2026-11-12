#include "../util/winmanager.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
//    this->wid_login = new loginWidget;
    this->setWindowTitle("监控系统主页面");
    this->setMinimumSize(1600,1000);
    resize(1600,1000);

    //窗口主布局采集垂直布局
    QVBoxLayout* main_layout = new QVBoxLayout;
    this->setLayout(main_layout);

    //顶部导航栏布局
    top_layout = new QHBoxLayout;
    //下方窗口

    m_pages = new QStackedLayout;

    m_monitorPage = new MonitorWidget;
    m_videoPage = new VideoWidget;
    m_zhuizongPage = new QWidget;   // 先占位，后面换成真正的页面
    m_logPage = new QWidget;
    m_settingPage  = new QWidget;

    m_pages->addWidget(m_monitorPage);    // index 0
    m_pages->addWidget(m_videoPage);      // index 1
    m_pages->addWidget(m_zhuizongPage);   // index 2
    m_pages->addWidget(m_logPage);        // index 3
    m_pages->addWidget(m_settingPage);    // index 4
    m_pages->setCurrentIndex(0);

    //将上面两个布局都加入到主布局容器中
    main_layout->addLayout(top_layout,1);
    main_layout->addLayout(m_pages, 8);

    //初始化顶部导航窗口
    this->init_top_widget();
    this->init_qss();
    this->init_connect();
    this->init_showtime();
}

void MainWidget::showUsername(const QString username)
{
    qDebug() << "showUsername";
    lab_login->setText(username);
    WinManager::getInstence()->loginWidet.hide();
    this->show();
}

/*
函数名：init_showtime
函数参数：无
函数功能：
1. 创建定时器，间隔 1000ms
2. 绑定信号槽：每秒触发update_time()
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

void MainWidget::init_top_widget()
{
    QLabel *lab_title = new QLabel("安防监控系统管理平台");
    lab_title->setObjectName("title");
    this->top_layout->addWidget(lab_title);
    this->top_layout->addStretch();

    btn_jiankong = new QPushButton(QIcon(":/image/jiankong.png"),"监控主界面");
    this->top_layout->addWidget(btn_jiankong);

    btn_review = new QPushButton(QIcon(":/image/huifang.png"),"普通视频回看");
    this->top_layout->addWidget(btn_review);

    btn_zhuizong = new QPushButton(QIcon(":/image/zhuizong.png"),"移动侦测回看");
    this->top_layout->addWidget(btn_zhuizong);

    btn_log = new QPushButton(QIcon(":/image/rizhi.png"),"日志");
    this->top_layout->addWidget(btn_log);

    btn_setting = new QPushButton(QIcon(":/image/seting.png"),"系统设置");
    this->top_layout->addWidget(btn_setting);

    lab_time = new QLabel("");
    top_layout->addWidget(lab_time);
    lab_time->setObjectName("ctime");

    QWidget *btn_login_box = new QWidget;
    QVBoxLayout* btn_login_layout = new QVBoxLayout(btn_login_box);

    btn_login = new QPushButton("");

    btn_login->setIcon(QIcon(":/image/user.png"));
    btn_login->setIconSize(QSize(40,50));
    btn_login_layout->addWidget(btn_login);

    lab_login = new QLabel("未登录");

    lab_login->setAlignment(Qt::AlignCenter);

    btn_login_layout->addWidget(lab_login);
    btn_login_layout->addStretch();
    top_layout->addWidget(btn_login_box);
}

void MainWidget::init_qss()
{
    QFile file(":/qss/mian.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void MainWidget::update_time()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("yyyy-MM-dd HH:mm:ss");
    lab_time->setText(timeString);
}

void MainWidget::init_connect()
{
    connect(btn_jiankong, &QPushButton::clicked, this, [this]{ m_pages->setCurrentIndex(0); });
    connect(btn_review, &QPushButton::clicked, this, [this]{ m_pages->setCurrentIndex(1); });
    connect(btn_zhuizong, &QPushButton::clicked, this, [this]{ m_pages->setCurrentIndex(2); });
    connect(btn_log, &QPushButton::clicked, this, [this]{ m_pages->setCurrentIndex(3); });
    connect(btn_setting, &QPushButton::clicked, this, [this]{ m_pages->setCurrentIndex(4); });
    connect(btn_login, &QPushButton::clicked, this, &MainWidget::to_login_win);
}

void MainWidget::to_login_win()
{
    WinManager::getInstence()->loginWidet.show();
}















