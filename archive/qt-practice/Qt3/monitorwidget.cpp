#include "monitorwidget.h"

MonitorWidget::MonitorWidget(QWidget *parent) : QWidget{parent}
{
    main_layout = new QHBoxLayout(this);
    W_left = new QWidget;
    W_right = new QWidget;

    main_layout->addWidget(W_left);
    main_layout->addWidget(W_right);

    this->init_qss();
    this->init_leftWidget();
    this->init_rightWidget();
    this->init_connect();
}

/*
函数名：
函数参数：
函数功能：
*/
void MonitorWidget::init_leftWidget()
{
    QVBoxLayout* left_layout = new QVBoxLayout(W_left);
    W_left->setFixedWidth(220);
    QLabel* lab_devicelist = new QLabel("设备列表");
    lab_devicelist->setAlignment(Qt::AlignCenter);
    lab_devicelist->setObjectName("devicelist");
    left_layout->addWidget(lab_devicelist);

    QListWidget* device_list = new QListWidget;
    left_layout->addWidget(device_list);
    QListWidgetItem* item1 = new QListWidgetItem(QIcon(":/imge/camara.png"),"相机1");
    device_list->addItem(item1);
    QListWidgetItem* item2 = new QListWidgetItem(QIcon(":/imge/camara.png"),"相机2");
    device_list->addItem(item2);
    QListWidgetItem* item3 = new QListWidgetItem(QIcon(":/imge/camara.png"),"相机3");
    device_list->addItem(item3);

    left_layout->addStretch();

}

/*
函数名：
函数参数：
函数功能：
*/
void MonitorWidget::init_rightWidget()
{
    // 上方为视频监控区，下方是操作按钮，设置垂直布局
    QVBoxLayout* right_layout = new QVBoxLayout(W_right);

    //视频上方监控视频区为堆叠布局
    video_layout = new QStackedLayout;

    //创建单通道窗口
    QWidget* w1 = new QWidget;
    video_layout->addWidget(w1);

    //创建四通道窗口
    QWidget* w4 = new QWidget;
    video_layout->addWidget(w4);

    right_layout->addLayout(video_layout);

    // 初始化单通道
    QVBoxLayout* w1_layout = new QVBoxLayout(w1);
    lab_vedeo1 = new QLabel;
    lab_vedeo1->setPixmap(QPixmap(":/imge/2.png"));
    lab_vedeo1->setScaledContents(true);
    lab_vedeo1->setAlignment(Qt::AlignCenter);
    w1_layout->addWidget(lab_vedeo1,0,Qt::AlignCenter);

    //初始化四通道
    int x = 0,y = 0;
    QGridLayout* w4_layout = new QGridLayout(w4);
    for(int i = 0; i < 4; i++)
    {
        lab_vedeo[i] = new QLabel;
        lab_vedeo[i]->setPixmap(QPixmap(":/imge/2.png"));
        lab_vedeo[i]->setScaledContents(true);   //内容随窗口缩放
        lab_vedeo[i]->setAlignment(Qt::AlignCenter);  //设置内容居中
        switch(i)
        {
        case 0: x = y = 0; break;
        case 1: x = 0; y = 1; break;
        case 2: x = 1; y = 0; break;
        case 3: x = 1; y = 1; break;
        }
        w4_layout->addWidget(lab_vedeo[i], x, y);
    }

    video_layout->setCurrentIndex(0);  //优先窗口显示
    //下方按钮区
    QHBoxLayout* btn_layout = new QHBoxLayout;
    btn_channel1 = new QPushButton(QIcon(":/imge/1.png"), "");
    btn_channel4 = new QPushButton(QIcon(":/imge/4.png"), "");
    btn_channel1->setIconSize(QSize(80, 80));
    btn_channel4->setIconSize(QSize(80, 80));
    btn_layout->addWidget(btn_channel4);
    btn_layout->addWidget(btn_channel1);
    btn_layout->addStretch(1);
    btn_layout->setSpacing(30);

    right_layout->addLayout(btn_layout);
}

/*
函数名：
函数参数：
函数功能：
*/
void MonitorWidget::init_qss()
{
        QFile file(":/qss/Monitor.qss");
        if(file.open(QFile::ReadOnly))
        {
            this->setStyleSheet(file.readAll());
            file.close();
        }
}

/*
函数名：
函数参数：
函数功能：
*/
void MonitorWidget::switch_to_channel4()
{
    video_layout->setCurrentIndex(0);
}

/*
函数名：
函数参数：
函数功能：
*/
void MonitorWidget::switch_to_channel1()
{
    video_layout->setCurrentIndex(1);
}


void MonitorWidget::init_connect()
{
    connect(btn_channel1, SIGNAL(clicked()), this, SLOT(switch_to_channel4()));
    connect(btn_channel4, SIGNAL(clicked()), this, SLOT(switch_to_channel1()));
}














