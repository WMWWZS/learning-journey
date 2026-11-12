#include "CameraWidget.h"

CameraWidget::CameraWidget(QWidget *parent) : QWidget{parent}
{
    //1.初始化控件和布局
    lab_show = new QLabel;
    btn_off = new QPushButton("关闭摄像头");
    btn_on = new QPushButton("打开摄像头");
    btn_record = new QPushButton("开启录制");
    this->is_record = false;
    btn_off->setEnabled(false);
    btn_record->setEnabled(false);
    cameraTask = nullptr;
    lab_show->setStyleSheet("background-color: black;");

    QVBoxLayout *V_layout = new QVBoxLayout(this);
    QHBoxLayout *H_layout = new QHBoxLayout;

    V_layout->addWidget(lab_show);
    H_layout->addWidget(btn_on);
    H_layout->addWidget(btn_off);
    H_layout->addWidget(btn_record);

    V_layout->addLayout(H_layout);

    connect(btn_on, &QPushButton::clicked, this, &CameraWidget::open_camera);
    connect(btn_off, &QPushButton::clicked, this, &CameraWidget::close_camera);
    connect(btn_record, &QPushButton::clicked, this, &CameraWidget::start_record);
}

QImage CameraWidget::cvMat2QImage(const cv::Mat &mat)
{
    if(mat.type() == CV_8UC3)
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        QImage img(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        return img.copy();
    }
    return QImage();
}
void CameraWidget::open_camera()
{
    // 只能有一个摄像头任务
    if (cameraTask != nullptr)
    {
        close_camera();
    }
    //1.开启工作线程
    QThread *cameraTask_QThread = new QThread;
    cameraTask = new CameraTask;
    cameraTask->moveToThread(cameraTask_QThread);

    //2.连接槽与信号
    connect(cameraTask, &CameraTask::sig_videoFrame, this, &CameraWidget::show_frame);  //处理帧和接收帧
    connect(cameraTask_QThread, &QThread::started, cameraTask, &CameraTask::open_camera);
    connect(cameraTask, &CameraTask::destroyed, cameraTask_QThread, &QThread::quit);
    connect(cameraTask_QThread, &QThread::finished, cameraTask_QThread, &QObject::deleteLater);

    cameraTask_QThread->start();

    btn_on->setEnabled(false);
    btn_off->setEnabled(true);
    btn_record->setEnabled(true);
}

void CameraWidget::close_camera()
{
    //1.先确保关闭录像
    if(is_record)
    {
        if(videoWriter != nullptr && videoWriter->isOpened())
        {
            videoWriter->release();  // 刷新缓存 写入磁盘 完成保存
            delete videoWriter;
            videoWriter = nullptr;
        }
        //放一张默认黑图片占位
        QPixmap framePix;
        lab_show->setPixmap(framePix.scaled(lab_show->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        is_record = false;
        btn_record->setText("开启录制");
    }
    //2.再停止摄像头
    if (cameraTask != nullptr)
    {
        cameraTask->stop_camera();
        cameraTask->deleteLater();
        cameraTask = nullptr;
    }
    //3.按键状态更新
    btn_on->setEnabled(true);
    btn_off->setEnabled(false);
    btn_record->setEnabled(false);
    QPixmap framePix;
    lab_show->setPixmap(framePix.scaled(lab_show->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void CameraWidget::start_record()
{
    if(is_record) //已处于录制状态的点击模式
    {
        btn_record->setEnabled(true);
        videoWriter->release();
        delete videoWriter;
        videoWriter = nullptr;
        btn_record->setText("开启录制");
        this->is_record = false;
    }
    else
    {
        QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + "_record.mp4";
        path_name = fileName.toStdString();  //思考为什么？
        int fps = 30;
        Size frameSize(cameraTask->getWidth(), cameraTask->getHeight());
        //设定视频编码
        int fourcc = VideoWriter::fourcc('H', '2', '6', '4');
        std::string savePath = "../records/" + path_name;
        videoWriter = new VideoWriter(savePath, fourcc, fps, frameSize, true);
        if(!videoWriter->isOpened()) //空就是结束flase
        {
            delete videoWriter;
            videoWriter = nullptr;
            return;
        }
        qDebug() << "视频写入器初始化成功，保存的文件名为：" << path_name;
        btn_record->setText("结束录制");
        // 按钮待启动状态
        this->is_record = true;
    }
}

void CameraWidget::show_frame(Mat frame)
{
    //录制状态 是-对视频写入器写入帧数同时投放 否-直接投放图片
    if(is_record)
    {
        videoWriter->write(frame);
    }

    QImage img = cvMat2QImage(frame);
    if(!img.isNull()){
        QPixmap framePix = QPixmap::fromImage(img);
        lab_show->setPixmap(framePix.scaled(lab_show->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        lab_show->setAlignment(Qt::AlignCenter);
    }
}


