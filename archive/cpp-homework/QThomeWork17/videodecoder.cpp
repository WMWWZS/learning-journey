#include "videodecoder.h"


VideoDecoder::VideoDecoder(QObject *parent) //初始化参数
    : QObject{parent}, m_stop(false), m_seeking(false), m_seekFrame(0), m_speed(1)
{}


VideoDecoder::~VideoDecoder()
{
    m_cap.release();
}

void VideoDecoder::startDecode(const QString &filePath)
{
    m_stop = false;
    m_cap.open(filePath.toStdString());
    //1.打开视频
    if(!m_cap.isOpened())
    {
        qDebug() << "子线程视频打开失败";
        emit decoderFinished();
        return;
    }

    //2.调整延时
    double fps = m_cap.get(cv::CAP_PROP_FPS);
    int baseDelay = static_cast<int>(1000.0 / fps); //基础延时（1倍速）

    cv::Mat frame;
    while (!m_stop)
    {
        //是否需要跳转
        if (m_seeking)
        {
            m_cap.set(cv::CAP_PROP_POS_FRAMES, m_seekFrame);
            m_seeking = false;
        }
        //视频已读取完毕
        if (!m_cap.read(frame)) {
            emit decoderFinished();
            break;
        }

        //百分比定位
        cv::putText(frame, "WuShiShuai",
                    cv::Point(static_cast<int>(frame.cols * 0.02),
                              static_cast<int>(frame.rows * 0.08)),
                    cv::FONT_HERSHEY_SIMPLEX, 1.0,
                    cv::Scalar(255, 50, 150), 2, cv::LINE_AA);

        //转换为QImage
        QImage img;
        if (frame.type() == CV_8UC3)
        {
            img = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888).copy();
        }
        else
        {
            continue;
        }

        emit sendFrame(img);

        // 通知主线程更新滑块位置
        int curFrame = static_cast<int>(m_cap.get(cv::CAP_PROP_POS_FRAMES));
        emit currentFrameChanged(curFrame);

        int delay = static_cast<int>(baseDelay / m_speed);
        if (delay < 1)
            delay = 1;
        QThread::msleep(delay);
    }

    m_cap.release();
    emit decoderFinished();
}

void VideoDecoder::stopDecode()
{
    m_stop = true;
}

void VideoDecoder::seekTo(int frameNo)
{
    if (frameNo < 0)
        return;
    m_seekFrame = frameNo;
    m_seeking = true;
}

void VideoDecoder::setSpeed(double speed)
{
    if (speed <= 0.0)
        return;
    m_speed = speed;
}
