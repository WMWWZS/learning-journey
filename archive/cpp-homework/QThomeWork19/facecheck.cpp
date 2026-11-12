#include "facecheck.h"

FaceCheck::FaceCheck(QWidget *parent) : QWidget{parent}
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout;
    //复选框
    m_checkFace = new QCheckBox("人脸识别", this);

    //画面显示
    m_labelShow = new QLabel;
    m_labelShow->setMinimumSize(640, 480);
    m_labelShow->setStyleSheet("background-color:black;");

    //水平布局
    hLayout->addStretch();
    hLayout->addWidget(m_checkFace);
    hLayout->addStretch();

    //加入垂直布局
    vLayout->addWidget(m_labelShow);
    vLayout->addLayout(hLayout);
    vLayout->setSpacing(10);

    //工作线程
    m_thread = new QThread(this);
    m_task = new FaceCheckTask;              // 不能给 parent，否则 moveToThread 会失败
    m_task->moveToThread(m_thread);


    //线程启动
    connect(m_thread, &QThread::started, m_task, FaceCheckTask::start);
    //线程销毁
    connect(m_thread, &QThread::finished, m_task, &QObject::deleteLater);

    //复选框连接到子线程
    connect(m_checkFace, &QCheckBox::toggled, m_task, &FaceCheckTask::setFaceEnable);

    //子线程传递图片产物给界面show
    connect(m_task, &FaceCheckTask::sig_frameReady, this, &FaceCheck::onFrameReady);
    connect(m_task, &FaceCheckTask::sig_error, this, &FaceCheck::onError);

    m_thread->start();
}


QImage FaceCheck::vcvMat2QImage(const cv::Mat &mat)
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

void FaceCheck::onFrameReady(const QImage &image)
{
    if(image.isNull())
    {
        return;
    }
    m_labelShow->setPixmap(QPixmap::fromImage(image)  //QImage是界面无关的像素容器，QPixmap 是显示优化的绘制格式
    .scaled(m_labelShow->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void FaceCheck::onError(const QString &message)
{
    qDebug() << "[FaceCheck]" << message;
    m_labelShow->setText(message);
}
