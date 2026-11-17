#ifndef RECORDTASK_H
#define RECORDTASK_H

#include <QDateTime>    // 记录每段视频的开始/结束时间
#include <QMap>         // 用通道号查每一路的状态
#include <QObject>      // 信号槽基类
#include <QSqlDatabase> // 数据库连接
#include <QString>

#include <opencv2/opencv.hpp>
//把收到的帧写成视频文件，并往数据库里记一条记录

struct channelWrite
{
    cv::VideoWriter writer; //写视频的对象
    QString fileName; //入库用的文件名
    //Record_20261030_231745_ch1.mp4
    QString filePath;  //完整路径
    QDateTime startTime; //这一段的开始时间
    int frameCount = 0; //这一段已经写了多少帧
    int targetFrame = 0;  //这一段应该写多少帧
    double fps = 30.0;
    cv::Size size; //这一段的分辨率
};

/**
 * @brief 多通道录制任务
 *
 * 用法：
 *   1 把对象moveToThread到一个 QThread
 *   2 线程started时调用init()（建立数据库连接）
 *   3 每收到一帧就调用save_frame(frame,通道号,fps)
 *   4 关闭程序前调用finish()（把最后不满一段的视频收尾并入库）
 */

class RecordTask : public QObject
{
    Q_OBJECT
public:
    explicit RecordTask(int m_segmentSeconds = 60 ,QObject *parent = nullptr);

    ~RecordTask() override;

signals:
    void sig_error(const QString &message);
    void sig_segment_finished(int channelId, const QString &path);


public slots:
    void init();   // 由QThread::started触发：在这个线程里建数据库连接

    void save_frame(cv::Mat frame, int channelId, double fps);

    void finish();

private:
    bool ensureDb(); //确保数据库连接
    bool openSegment(int channelId, const cv::Mat &frame, double fps);//开一个新文件
    void closeSegment(int channelId);  //收尾+入库
    bool insertRecord(int channelId, const channelWrite &w);  //往video表中插入
    QString buildPath(int channelId, const QDateTime &time);  //文件路径拼接

    QMap<int, channelWrite> m_writers;  //key=通道号，value=这一路的信息
    QSqlDatabase m_db;                   //本线程自己的数据库连接
    int m_segmentSeconds = 60;  //每段秒数
    QString m_rootDir;              //根目录：<exe目录>/records
};

#endif // RECORDTASK_H
