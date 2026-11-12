#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
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
    this->show();
}

void MainWidget::init_top_widget()
{
    QLabel *lab_title = new QLabel("安防监控系统管理平台");
    lab_title->setObjectName("title");
    this->top_layout->addWidget(lab_title);
    this->top_layout->addStretch();

    QPushButton *btn_jiankong = new QPushButton(QIcon(":/image/jiankong.png"),"监控");
    this->top_layout->addWidget(btn_jiankong);
    QPushButton *btn_review = new QPushButton(QIcon(":/image/huifang.png"),"回放");
    this->top_layout->addWidget(btn_review);
    QPushButton *btn_setting = new QPushButton(QIcon(":/image/seting.png"),"设置");
    this->top_layout->addWidget(btn_setting);

    QLabel *lab_time = new QLabel("2026-08-14 17:00:04");
    top_layout->addWidget(lab_time);
    lab_time->setObjectName("ctime");

    QWidget *btn_login_box = new QWidget;
    QVBoxLayout* btn_login_layout = new QVBoxLayout;
    btn_login_box->setLayout(btn_login_layout);
    QPushButton *btn_login = new QPushButton("");
    btn_login->setIcon(QIcon(":/image/user.png"));
    btn_login->setIconSize(QSize(40,50));
    btn_login_layout->addWidget(btn_login);
    QLabel* lab_login = new QLabel("未登录");
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
