#include "videoplay.h"

VideoPlay::VideoPlay(QWidget *parent) : QWidget{parent}
{
    // 1. 布局创建
    QVBoxLayout* Vlay = new QVBoxLayout();
    QHBoxLayout* Hlay = new QHBoxLayout();

    // 2. 控件初始化
    m_btn_play = new QPushButton("播放");
    m_label_show = new QLabel("等待视频中...");
    m_label_show->setAlignment(Qt::AlignCenter);
    m_timer = new QTimer(this);
    m_slider = new QSlider(Qt::Horizontal);

    //倍速控件
    m_combo_speed = new QComboBox;
    m_combo_speed->addItem("0.5x", 0.5);
    m_combo_speed->addItem("1.0x", 1.0);
    m_combo_speed->setCurrentIndex(1); //默认1.0倍
    m_combo_speed->addItem("1.5x", 1.5);
    m_combo_speed->addItem("2.0x", 2.0);

    // 3.布局组装
    Hlay->addWidget(m_btn_play);
    Hlay->addWidget(m_combo_speed); // 播放按钮和倍速放一行

    Vlay->addWidget(m_label_show);
    Vlay->addWidget(m_slider);
    Vlay->addLayout(Hlay);

    setLayout(Vlay);

    m_cap.open("../video/ovals_04.mp4");
    if(!m_cap.isOpened()) {
        qDebug() << "视频打开失败，请检查路径或工作目录";
        return;
    }
    //给滑动条设置总帧数
    m_total_frames = static_cast<long>(m_cap.get(cv::CAP_PROP_FRAME_COUNT));
    m_slider->setRange(0, static_cast<int>(m_total_frames));
    m_fps = m_cap.get(cv::CAP_PROP_FPS);
    //关闭主线程，仅仅获取视频的相关属性，解码由子线程搞定
    m_cap.release();

    //初始化线程
    m_decoder_QThread = new QThread;
    m_decoder = new VideoDecoder;
    m_decoder->moveToThread(m_decoder_QThread);

    //子线程处理数据给主线程
    //此处线程start()
    connect(m_decoder_QThread, &QThread::started, m_decoder, [this](){m_decoder->startDecode("../video/ovals_04.mp4");
    });

    connect(m_decoder, &VideoDecoder::sendFrame, this, &VideoPlay::slot_recvFrame);

    connect(m_decoder, &VideoDecoder::decoderFinished, this, &VideoPlay::slot_decoderFinished);

    connect(m_decoder, &VideoDecoder::currentFrameChanged, this, [this](int frame){
        m_slider->setValue(frame);});

    connect(this, &VideoPlay::requestSeek, m_decoder, &VideoDecoder::seekTo, Qt::DirectConnection);  // 用于解决滑块帧数无法定位的问题

    //按钮
    connect(m_btn_play, &QPushButton::clicked, this, &VideoPlay::slot_playPause);  //点击 暂停
    connect(m_slider, &QSlider::sliderMoved, this, &VideoPlay::slot_sliderDrag); //滑动 跳帧   注意：sliderMoved会传帧数位置序号，可能不准确
    connect(m_combo_speed, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VideoPlay::slot_speedChangedText);
}

//由于cv的Mat类是BRG需要转RBG在qt控件上展示
QImage VideoPlay::cvMat2QImage(const cv::Mat &mat)
{
    if(mat.type() == CV_8UC3)
    {
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.copy();
    }
    return QImage();
}

//废弃
void VideoPlay::slot_readFrame()
{
    cv::Mat frame;
    //读完了
    if(!m_cap.read(frame))
    {
        m_btn_play->setText("播放");
        m_timer->stop();
        m_is_playing = false;
        return;
    }

    cv::putText(frame, "WuShiShuai",cv::Point(static_cast<int>(frame.cols * 0.02),
    static_cast<int>(frame.rows * 0.08)),cv::FONT_HERSHEY_SIMPLEX,1.0,
    cv::Scalar(255, 50, 150),2,cv::LINE_AA);

    //没读完继续
    int curFrame = static_cast<int>(m_cap.get(cv::CAP_PROP_POS_FRAMES));   //获取实际读到的位置
    m_slider->setValue(curFrame);

    QImage img = cvMat2QImage(frame);
    m_label_show->setPixmap(QPixmap::fromImage(img).scaled(m_label_show->size(),Qt::KeepAspectRatio));
}

void VideoPlay::slot_playPause()
{
    //是否处于播放状态
    if(m_is_playing)
    {
        m_decoder->stopDecode();
        m_btn_play->setText("播放");
        m_is_playing = false;
    }
    else
    {
        if(!m_decoder_QThread->isRunning())  //如果线程未启动
        {
            m_decoder_QThread->start();
        }
        else
        {
            m_decoder_QThread->quit();
            m_decoder_QThread->wait();
            m_decoder_QThread->start();
        }
        m_btn_play->setText("暂停");
        m_is_playing = true;
    }
}

void VideoPlay::slot_sliderDrag(int frameNo) //自动接收sliderMoved信号传过来的position的值
{
    // //通知子线程跳转
    // qDebug() << "进入滑动条模式";
    // m_decoder->seekTo(frameNo);   //bug记录这里只是主线程直接调用子线程对象的方法

    qDebug() << "请求跳转到第" << frameNo << "帧";
    emit requestSeek(frameNo);
}

void VideoPlay::slot_speedChangedText(int index)
{
    double speed = m_combo_speed->itemData(index).toDouble();
    qDebug() << "speed =" << speed;

    m_decoder->setSpeed(speed);
}

//接收到处理完的img进行show
void VideoPlay::slot_recvFrame(const QImage &img)
{
    m_label_show->setPixmap(QPixmap::fromImage(img).scaled(
    m_label_show->size(), Qt::KeepAspectRatio));
}

//视频完成全部解码自动暂停播放
void VideoPlay::slot_decoderFinished()
{
    m_btn_play->setText("播放");
    m_is_playing = false;
    qDebug() << "视频播放完毕";
}
