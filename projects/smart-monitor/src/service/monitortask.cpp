#include "monitortask.h"

#include <QDebug>
#include <QElapsedTimer> //高精度计时器，用来做按帧率节流
#include <QThread>       //提供msleep/usleep这类睡眠函数
#include <stdio.h>
MonitorTask::MonitorTask(
    const QString &source,
    const QString &fallbackFile,  //默认空 = 不启用回退
    int channelId, //1~4
    QObject *parent) :
    QObject(parent)   // 把parent交给QObject管理（parent析构时会删掉它）
    , m_source(source)
    , m_lastFile(fallbackFile)
    , m_channelId(channelId)
{
    //先判断是文件还是摄像头：约定"file:"开头的就是文件
    m_isFile = source.startsWith("file:");
}

MonitorTask::~MonitorTask()
{
    releaseCap();
    qDebug() << "摄像头已析构";
}

bool MonitorTask::openSource()
{
    if(m_isFile) //如果是视频文件
    {
        //去掉前面的"file:"，剩下的就是路径
        const QString path = m_source.mid(5);
        m_cap.open(path.toStdString());
        if(m_cap.isOpened())
        {
            qDebug()<< "file:" << path << "文件已成功打开";
        }
    }
    else //如果是摄像头 cam:0 是摄像头
    {
        const int index = m_source.mid(4).toInt();  //去掉0123的索引得到 int 0
        m_cap.open(index);
        if(m_cap.isOpened())
        {
            qDebug()<< "摄像头已成功打开";
        }
    }

    // 摄像头/文件都打不开
    if(!m_cap.isOpened() && !m_lastFile.isEmpty())
    {
        m_isFile = true;   //开启"文件模式"
        m_cap.open(m_lastFile.toStdString(), cv::CAP_FFMPEG);
        qWarning() << "channel" << m_channelId << "视频文件和摄像头无法打开，当前转到备用文件"
        << m_lastFile;
    }

    return m_cap.isOpened();
}


// 摄像头模式的循环
// 摄像头本身按硬件帧率出帧，所以不需要额外sleep节流
void MonitorTask::loopCamera()
{
    cv::Mat frame;
    int failCount = 0;  //记录异常的连续多帧，那应该就是采集出问题了
    while(!m_stop)
    {
        // read()成功时会往frame里填数据失败（拔线/被占用）返回false或空帧，就是设备读帧异常积累
        if(!m_cap.read(frame) || frame.empty())
        {
            if(++failCount > 30)  //触发重连
            {
                releaseCap();
                openSource();
                failCount = 0;
            }
            QThread::msleep(100);
            continue;
        }
        failCount = 0;
        emit send_frame(frame.clone(), m_channelId, m_fps);
    }
}

//视频文件节流模式
void MonitorTask::loopFile()
{
    cv::Mat frame;
    QElapsedTimer clock;
    clock.start();          // 从0开始计时
    qint64 nextUs = 0;      // 下一帧“应该”在什么时刻到达（微秒）

    while(!m_stop)
    {
        if(!m_cap.read(frame) && frame.empty())
        {
            // 文件播完了->跳回第0帧，循环播放
            if (!m_cap.set(cv::CAP_PROP_POS_FRAMES, 0))
            {
                break;
            }
            continue;
        }

        emit send_frame(frame.clone(), m_channelId, m_fps);

        // 计算下一帧的目标时间：每帧间隔 = 1e6 / fps 微秒
        nextUs += static_cast<qint64>(1e6 / m_fps);
        //已经过了多久（纳秒转微秒），还差多少
        const qint64 waitUs = nextUs - clock.nsecsElapsed() / 1000;
        // 还没到时间就睡一会儿如果已经落后了就不睡（尽量追帧）
        if (waitUs > 0) {
            QThread::usleep(static_cast<unsigned long>(waitUs));
        }
    }
}

void MonitorTask::releaseCap()
{
    if (m_cap.isOpened())
    {
        m_cap.release();   // 释放设备/文件句柄
    }
}

void MonitorTask::start()
{
    if(!openSource())
    {
        emit sig_error(m_channelId, QString("打开失败：%1").arg(m_source));
        return;
    }

    if (!m_isFile)
    {
        // 这里只是请求640x480 驱动不一定答应，最终以实际读到的帧大小为准
        m_cap.set(cv::CAP_PROP_FRAME_WIDTH,  m_camWidth);
        m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, m_camHeight);
        cv::Mat warm;
        for (int i = 0; i < 5 && !m_stop; ++i)
        {
            m_cap.read(warm);   // 读出来直接扔掉（不emit）刚开始是糊的
        }
    }

    const double srcFps = m_cap.get(cv::CAP_PROP_FPS);

    // 小于等于1就当作30处理
    m_fps = (srcFps > 1) ? srcFps : 30.0;

    m_size = cv::Size(static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH)),
                      static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

    //告诉外界已准备就绪
    emit sig_opened(m_channelId, m_fps, m_size.width, m_size.height);

    // 就绪信息
    qDebug() << "channel" << m_channelId << "已就绪成功,"
             << m_size.width << "x" << m_size.height << " " << m_fps << "fps"
             << (m_isFile ? "(file)" : "(camera)");

    //分配模式
    if (m_isFile)
    {
        loopFile();
    }
    else
    {
        loopCamera();
    }

    //结束
    releaseCap();
}

// 请求停止 只做一件事：把原子标志置true
void MonitorTask::stop()
{
    m_stop = true;
}


