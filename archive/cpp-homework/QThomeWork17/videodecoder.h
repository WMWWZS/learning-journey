#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QString>
#include <Qdebug>
#include <QThread>

class VideoDecoder : public QObject
{
    Q_OBJECT

public:
    explicit VideoDecoder(QObject *parent = nullptr);

    ~VideoDecoder();
    QImage cvMat2QImage(const cv::Mat &mat);
signals:
    void sendFrame(const QImage& img);  //发送解码后的图像
    void decoderFinished();   //解码完成
    void currentFrameChanged(int frameNo); //更新当前的针 用于滑动

public slots:
    void startDecode(const QString &filePath);  //开始解码
    void stopDecode();  // 停止解码
    void seekTo(int frameNo); //跳转到指定帧
    void setSpeed(double speed);  //设置倍速

private:
    cv::VideoCapture m_cap;
    volatile bool m_stop;        // 停止标志
    volatile bool m_seeking;     // 是否有跳转请求
    volatile int m_seekFrame;   // 要跳到的目标帧
    volatile double m_speed;
};

#endif // VIDEODECODER_H
