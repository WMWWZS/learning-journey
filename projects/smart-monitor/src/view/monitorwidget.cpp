#include "monitorwidget.h"

#include <QCoreApplication>

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
    for (int i = 0; i < 4; ++i)
    {
        m_labels[i] = lab_vedeo[i];
    }
    this->init_connect();
    this->start_palyer();
}

MonitorWidget::~MonitorWidget()
{
    stop_all();
}

QImage MonitorWidget::MatToQImage(const Mat& frame)
{
    if(frame.empty())
    {
        return QImage();
    }
    Mat temp;
    switch(frame.type())
    {
    case CV_8UC1:
        return QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_Grayscale8).copy();
    case CV_8UC3:
        cvtColor(frame, temp, COLOR_BGR2RGB);
        return QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888).copy();
    default:
        return QImage();
    }
}

void MonitorWidget::stop_all()
{
    for (auto *task : m_tasks)
    {
        QMetaObject::invokeMethod(task, "stop", Qt::DirectConnection);
    }

    for (auto *thread : m_threads) {
        thread->quit();       //请求事件循环退出
        thread->wait(3000);   //等待3秒
    }
    m_tasks.clear();
    m_threads.clear();

    if (m_recordTask)
    {
        QMetaObject::invokeMethod(m_recordTask, "finish", Qt::BlockingQueuedConnection);
    }
    if (m_recordThread) {
        m_recordThread->quit();
        m_recordThread->wait(3000);
        m_recordThread->deleteLater();   // 交给事件循环安全释放
    }
}

void MonitorWidget::init_leftWidget()
{
    QVBoxLayout* left_layout = new QVBoxLayout(W_left);
    W_left->setFixedWidth(220);
    W_left->setObjectName("left_box");

    QLabel* lab_devicelist = new QLabel("设备列表");
    lab_devicelist->setAlignment(Qt::AlignCenter);
    lab_devicelist->setObjectName("device_list");
    left_layout->addWidget(lab_devicelist);
    QListWidget* device_list = new QListWidget;
    left_layout->addWidget(device_list);
    QListWidgetItem* item1 = new QListWidgetItem(QIcon(":/image/camara.png"),"摄像头1");
    device_list->addItem(item1);
    QListWidgetItem* item2 = new QListWidgetItem(QIcon(":/image/camara.png"),"摄像头2");
    device_list->addItem(item2);
    QListWidgetItem* item3 = new QListWidgetItem(QIcon(":/image/camara.png"),"摄像头3");
    device_list->addItem(item3);

    left_layout->addStretch();
}


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
    w1_layout->setContentsMargins(0, 0, 0, 0);

    lab_vedeo1 = new QLabel;
    lab_vedeo1->setMinimumSize(640, 480);
    lab_vedeo1->setAlignment(Qt::AlignCenter);
    lab_vedeo1->setScaledContents(true);              //自己算缩放，不要让label拉伸
    lab_vedeo1->setStyleSheet("background:black;");
    w1_layout->addWidget(lab_vedeo1, 1);                // ← stretch=1，占满整个页面

    //初始化四通道
    int x = 0,y = 0;
    QGridLayout* w4_layout = new QGridLayout(w4);
    for(int i = 0; i < 4; i++)
    {
        lab_vedeo[i] = new QLabel;
        lab_vedeo[i]->setPixmap(QPixmap(":/image/2.png"));
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
    video_layout->setCurrentIndex(1);  //优先窗口显示
    //下方按钮区
    QHBoxLayout* btn_layout = new QHBoxLayout;
    btn_channel1 = new QPushButton(QIcon(":/image/1.png"), "");
    btn_channel4 = new QPushButton(QIcon(":/image/4.png"), "");
    btn_channel1->setIconSize(QSize(80, 80));
    btn_channel4->setIconSize(QSize(80, 80));
    btn_layout->addWidget(btn_channel4);
    btn_layout->addWidget(btn_channel1);
    btn_layout->addStretch(1);
    btn_layout->setSpacing(30);


    right_layout->addLayout(btn_layout);
}

void MonitorWidget::init_qss()
{
        QFile file(":/qss/Monitor.qss");
        if(file.open(QFile::ReadOnly))
        {
            this->setStyleSheet(file.readAll());
            file.close();
        }
}

void MonitorWidget::switch_to_channel4()
{
    video_layout->setCurrentIndex(1);
}

void MonitorWidget::switch_to_channel1()
{
    video_layout->setCurrentIndex(0);
}

void MonitorWidget::start_palyer()
{
    m_recordTask   = new RecordTask(60);   // 每段 60 秒；测试可改 10
    m_recordThread = new QThread(this);
    m_recordTask->moveToThread(m_recordThread);

    connect(m_recordThread, &QThread::started, m_recordTask,
            &RecordTask::init);

    connect(m_recordTask, &RecordTask::sig_error, this,
            [](const QString &msg){ qDebug() << msg;}
            );

    m_recordThread->start();

    struct ChannelConfig {
        QString source;     //cam:0或 file:路径
        QString fallback;   //仅摄像头用：打不开时播这个视频，防止界面空一格
    };

    const ChannelConfig cfg[4] = {
        // 1号：笔记本摄像头；被微信/钉钉占用或没有摄像头时，回退到 CAM-01 视频
        { "cam:0", "C:/Users/风林火山/Desktop/四路摄像头测试视频/CAM-01_1280x720_30fps.mp4" },
        { "file:C:/Users/风林火山/Desktop/四路摄像头测试视频/CAM-02_960x540_25fps.mp4",  "" },
        { "file:C:/Users/风林火山/Desktop/四路摄像头测试视频/CAM-03_640x480_30fps.mp4",  "" },
        // 2~4号：纯视频文件（换成你桌面那 4 个 CAM 视频的实际路径）
        { "file:C:/Users/风林火山/Desktop/四路摄像头测试视频/CAM-04_1280x720_25fps.mp4", "" }
    };

    for(int i = 0; i < 4; ++i)
    {
        // 通道号用i+1(1~4），和数据库channel_id对应
        auto *task = new MonitorTask(cfg[i].source, cfg[i].fallback, i + 1);
        auto *thread = new QThread(this);
        task->moveToThread(thread);


        // 线程启动 ->执行 task->start()（打开摄像头/文件，然后进入读帧循环）

        // 采集->录制
        connect(thread, &QThread::started, task, &MonitorTask::start);
        connect(task, &MonitorTask::send_frame,
                m_recordTask, &RecordTask::save_frame);

        //采集->界面显示
        connect(task, &MonitorTask::send_frame, this,
                [this](cv::Mat frame, int channelId, double)
                {
                    if (channelId < 1 || channelId > 4)
                        {
                        return;
                        }

                    QLabel *lab = m_labels[channelId - 1];
                    if (!lab)
                        {
                        return;
                        }

                    // 转QImage后缩放到label大小：
                    //   KeepAspectRatio->保持宽高比，不变形（会留黑边）
                    //FastTransformation->快速缩放，适合实时预览
                    lab->setPixmap(QPixmap::fromImage(MatToQImage(frame))
                                       .scaled(lab->size(),
                                               Qt::KeepAspectRatio,
                                               Qt::FastTransformation));  //参数代考据
                        if(channelId == 1)
                        {
                            lab_vedeo1->setPixmap(QPixmap::fromImage(MatToQImage(frame))
                                                      .scaled(lab->size(),
                                                              Qt::KeepAspectRatio,
                                                              Qt::FastTransformation));  //参数代考据
                        }

                });
        //出错时打印（哪个通道，什么错误）
        connect(task, &MonitorTask::sig_error, this,
                [](int ch, const QString &msg) { qDebug() << "[ch" << ch << "]" << msg; });

        m_tasks.append(task);
        m_threads.append(thread);
        thread->start();   // 真正开始采集
    }
}


void MonitorWidget::init_connect()
{
    connect(btn_channel1, SIGNAL(clicked()), this, SLOT(switch_to_channel1()));
    connect(btn_channel4, SIGNAL(clicked()), this, SLOT(switch_to_channel4()));
    connect(qApp, &QCoreApplication::aboutToQuit, this, &MonitorWidget::stop_all);
}















