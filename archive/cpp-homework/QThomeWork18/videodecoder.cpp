#include "videodecoder.h"

#include <QThread>




VideoDecoder::VideoDecoder(QObject *parent)
    : QObject(parent)
{

}

VideoDecoder::~VideoDecoder()
{
    m_cap.release();
}

void VideoDecoder::startDecode(const QString &filePath)
{
    m_stop = false;
    m_cap.open(filePath.toStdString());
    if(!m_cap.isOpened())
    {
        qDebug() << "子线程视频打开失败";
        emit decoderFinished();
        return;
    }

    double fps = m_cap.get(cv::CAP_PROP_FPS);
    int delay = static_cast<int>(1000 / fps);

    cv::Mat frame;
    while (!m_stop)
    {
        if (m_seeking) {
            //跳转期间，等待seekTo完成
            QThread::msleep(10);
            continue;
        }
        if (!m_cap.read(frame)) {
            //读完了
            emit decoderFinished();
            break;
        }

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

        //水印添加
        cv::putText(frame, "WuShiShuai",
                    cv::Point(static_cast<int>(frame.cols * 0.02),
                              static_cast<int>(frame.rows * 0.08)),
                    cv::FONT_HERSHEY_SIMPLEX, 1.0,
                    cv::Scalar(255, 50, 150), 2, cv::LINE_AA);

        emit sendFrame(img);

        // 通知主线主线程更新滑块位置
        int curFrame = static_cast<int>(m_cap.get(cv::CAP_PROP_POS_FRAMES));
        emit currentFrameChanged(curFrame);

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
    m_seeking = true;
    if(m_cap.isOpened())
    {
        m_cap.set(cv::CAP_PROP_POS_FRAMES, frameNo);  //写操作，向视频引擎发送请求：我想跳到第frameNo帧
        cv::Mat frame;
        if(m_cap.read(frame))
        {
            QImage img;
            if(frame.type() == CV_8UC3)
            {
                img = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format::Format_BGR888);
            }
            emit sendFrame(img);

            int curFrame = static_cast<int>(m_cap.get(cv::CAP_PROP_POS_FRAMES));
            emit currentFrameChanged(curFrame);
        }
    }
    m_seeking = false;
}
