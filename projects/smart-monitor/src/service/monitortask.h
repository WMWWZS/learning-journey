#ifndef MONITORTASK_H
#define MONITORTASK_H

#include <QObject>
#include <QString>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QImage>
#include <Qdebug>
/*
MonitorTask是视频和录制的采集器，放在一个独立的QThread里跑
不停地读取帧数，让后将信号发给两处：
1.界面（显示）
2.录制线程（写文件）
为什么要做这个类？
因为cap.read()是阻塞的，放在主线程（界面线程）里会让窗口卡死
鼠标点不动，所以读帧这种耗时的活必须扔到子线程
*/
using namespace cv;

class MonitorTask : public QObject
{
    Q_OBJECT
public:
    //构造
    explicit MonitorTask(const QString &source,
    const QString &fallbackFile = QString(),  //默认空 = 不启用回退
    int channelId = 1, //1~4
    QObject *parent = nullptr);

    //析构
    ~MonitorTask() override;

    //接口：三个简单的只读接口，外面可以查询这路的分辨率/帧率
    int channelId() const { return m_channelId; }
    double fps() const { return m_fps; }
    cv::Size frameSize() const { return m_size; }

signals:
    //每读取一帧就送一次，帧 通道号 此路帧率
    void send_frame(Mat frame, int channel_id, double fps);

    //每打开一次方便外界知道分辨率/帧率
    void sig_opened(int channelId, double fps, int width, int height);

    //打开失败/出错时发，界面可以显示提示
    void sig_error(int channelId, const QString &message);
private:
    bool openSource();  //打开摄像头或文件，失败返回false
    void loopCamera();  //摄像头模式地读帧模式
    void loopFile();    //文件模式地读帧模式
    void releaseCap();  //释放videoCapture;

    //配置
    QString m_source;    //打开cam:0 file:
    QString m_lastFile;  //摄像头打不开时的备胎视频
    int m_channelId = 1;  //1~4，用来给文件和数据库记录分通道
    bool m_isFile = false;  //当前到底在读文件还是摄像头

    //实际以cap.get(CAP_PROP_FRAME_WIDTH/HEIGHT)为准
    int m_camWidth = 640;
    int m_camHeight = 480;

    //为什么要用std::atomic_bool?
    /*
    因为涉及多线程（主线程的m_stop(用户操作)，子线程的m_stop(及其操作)）并发访问的问题
    同时访问一个m_stop就是可能会造成数据竞争，总之后果严重，用这个能够避免这些问题
    */
    std::atomic_bool m_stop{false};
    cv::VideoCapture m_cap;   //opencv的采集对象（摄像头/文件都要使用它)
    double m_fps = 30.0;  //这一路的帧率，写文件时要按它算 多少帧=60秒
    cv::Size m_size;        //这一路的分辨率

public slots:
    void start();
    void stop();
};

#endif // MONITORTASK_H
