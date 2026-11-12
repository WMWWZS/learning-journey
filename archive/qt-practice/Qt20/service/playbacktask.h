#ifndef PLAYBACKTASK_H
#define PLAYBACKTASK_H

//PlaybackTask：回看视频的解码任务
#include <QMutex>
#include <QObject>
#include <QWaitCondition>
#include <atomic>
#include <opencv2/opencv.hpp>
class PlaybackTask : public QObject
{
    Q_OBJECT
public:
    explicit PlaybackTask(QObject *parent = nullptr);
    ~PlaybackTask() override;
    //这里只是记下路径，等线程启动startDecode()时再真正打开
    void setSource(const QString &path) { m_path = path; }
signals:
    //当前读到第几帧：界面用它同步滑块位置
    void positionChanged(int frameNo);
    void finished();
    void frameReady(const QImage &framee);
    void error(const QString &message);
    void opened(double fps, int totalFrames);
private:
    // cv::Mat->QImage：BGR转RGB，并copy()让QImage拥有独立内存
    QImage matToImage(const cv::Mat &mat);

    //---解码相关---
    cv::VideoCapture m_cap;   //只在解码线程里 open/read/set/release
    QString m_path;           //要播放的文件路径（setSource/open写入）

    //---暂停退出---
    QMutex m_pauseMutex;
    QWaitCondition m_pauseCond;
    //---跨线程共享状态---
    std::atomic_bool m_stop{false};  //true = 循环要退出
    std::atomic_bool m_paused{false};    // true = 当前暂停
    std::atomic_bool m_pendingSeek{false};  //true = 有一次跳转请求待处理
    std::atomic_int m_seekFrame{0}; //跳转目标帧
    std::atomic<double> m_speed{1.0};   //倍速（0.5/1.0/2.0/4.0）

    //---视频属性---
    double m_fps = 25.0;  //文件帧率
    int m_totalFrames = 0;//总帧数
public slots:
    void open(const QString &path);
    void startDecode();
    void pause();   //暂停
    void stop();  //让while退出循环，线程结束
    void play(); // 继续/开始播放 清除暂停标志并唤醒等待中的解码线程
    void seekTo(); // 跳转到指定帧，只是记录请求，在解码线程中跳帧
    void setSpeed(double speed); //设置倍速，多线程访问用
};
//所有跨线程共享的变量都用std::atomic，不用volatile
#endif // PLAYBACKTASK_H
