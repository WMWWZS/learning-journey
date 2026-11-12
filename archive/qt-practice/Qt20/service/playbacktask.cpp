#include "playbacktask.h"
#include <QDebug>
#include <QImage>
#include <QMutexLocker>
#include <QThread>

PlaybackTask::PlaybackTask(QObject *parent) : QObject{parent}
{

}

PlaybackTask::~PlaybackTask()
{
    stop();            //告诉循环别再读了
    if (m_cap.isOpened())
    {
        m_cap.release();   //释放解码器
    }
}

QImage PlaybackTask::matToImage(const cv::Mat &mat)
{
    if(mat.empty() || mat.type() != CV_8UC3)  //只处理8位图
    {
        return QImage();
    }

    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    return QImage(rgb.data, rgb.cols,rgb.rows,static_cast<int>(rgb.step),   //step=一行多少字节（可能有内存对齐）
            QImage::Format_RGB888).copy();
}

//发出opened(m_fps, m_totalFrames);
void PlaybackTask::open(const QString &path)
{
    if(m_cap.isOpened())
    {
        m_cap.release();
    }

    m_path = path;
    m_cap.open(m_path.toStdString());  // OpenCV的open需要std::string

    if(!m_cap.isOpened())
    {
        emit error(QString("无法打开视频：%1").arg(m_path));
        return;
    }

    m_fps = m_cap.get(cv::CAP_PROP_FPS);  //文件写死的fps帧率
    if (m_fps <= 1.0)
    {
        m_fps = 25.0;
    }

    m_totalFrames = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_COUNT));
    if (m_totalFrames <= 0)
    {
        m_totalFrames = 1;
    }

    m_pendingSeek = false;
    emit opened(m_fps, m_totalFrames);
}

//发出finished(); frameReady(img);  positionChanged(pos);
void PlaybackTask::startDecode()
{
    //1.打开，videoCapture如果之前打开过，可以直接打开
    if(!m_cap.isOpened())
    {
        open(m_path);
        if(!m_cap.isOpened())
        {
            emit finished();   //打开失败，通知结束
        }
    }

    //2.打开成功进入读取
    cv::Mat frame;
    while(!m_stop)
    {
        {
            QMutexLocker locker(&m_pauseMutex);
            while (m_paused && !m_stop) //标记暂停，但循环不要求退出
            {
                m_pauseCond.wait(&m_pauseMutex); //让别的线程拿到锁，线程休眠，等待week
            }
        }
        if(m_stop)  //stop()可能是在暂停等待中被唤醒的，再检查一次
        {
            break;
        }

        if(m_pendingSeek.exchange(false)) //有跳转请求处理
        {
            m_cap.set(cv::CAP_PROP_POS_FRAMES, m_seekFrame.load()); //用最新的帧
        }

        if(!m_cap.read(frame) || frame.empty()) //读取一帧，读取失败/空针直接返回
        {
            emit finished();
            break;
        }

        const QImage img = matToImage(frame);
        if(img.isNull())
        {
            qDebug() << "playbacktask.cpp 74行错误：img.isNull()为真";
            continue;
        }

        emit frameReady(img);

        //当前帧号
        const int pos = static_cast<int>(m_cap.get(cv::CAP_PROP_POS_FRAMES)); //返回值安全转化为int类型
        emit positionChanged(pos);

        double speed = m_speed.load();  //保证多线程下能读到别的线程修改后的最新值

        if (speed < 0.1)
        {
            speed = 1.0;
        }

        int delay = static_cast<int>(1000.0 /(m_fps * speed));
        if (delay < 1)
        {
            delay = 1;   //高倍速时至少睡1ms，避免死循环占满CPU
        }
        QThread::msleep(static_cast<unsigned long>(delay));
        //speed越大，间隔delay越小，睡眠间隔越小，播放越快
    }
    //循环结束，释放解码器
    m_cap.release();
    //无论正常播完还是被stop，都通知界面恢复按钮状态
    emit finished();
}

void PlaybackTask::pause()
{
    QMutexLocker locker(&m_pauseMutex);
    m_paused = true;
}

void PlaybackTask::stop()
{
    m_stop = true;
    {
        QMutexLocker locker(&m_pauseMutex);
        m_paused = false;
    }
    m_pauseCond.wakeAll();
}

void PlaybackTask::play()
{
    {
        QMutexLocker locker(&m_pauseMutex);
        m_paused = false;      //清暂停标志
    }
    m_pauseCond.wakeAll();  //叫醒正在wait()的解码线程
}

void PlaybackTask::seekTo()
{
    QMutexLocker locker(&m_pauseMutex);
    m_paused = true;
}

void PlaybackTask::setSpeed(double speed)
{
    if (speed <= 0.0)
    {
        return;
    }
    m_speed = speed;
}