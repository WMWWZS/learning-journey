#ifndef FACECHECKTASK_H
#define FACECHECKTASK_H

#include <QObject>
#include <atomic>
#include <vector>
#include <opencv2/opencv.hpp>
#include "facedetector.h"
#include <QDir>
#include <QCoreApplication>
#include <QTimer>
#include <QImage>
#include <QDebug>

class FaceCheckTask : public QObject
{
    Q_OBJECT
public:
    explicit FaceCheckTask(QObject *parent = nullptr);

signals:
    void sig_frameReady(const QImage &image); //发给界面显示
    void sig_error(const QString &message);     // 出错提示

public slots:
    void start();     //线程启动后调用：开摄像头 开定时器
    void stop();           //线程退出前调用：关摄像头
    void setFaceEnable(bool enable);   //复选框状态：负责开/关人脸检测
    void onTimeout();   //帧数读取
private:
    static QImage matToQImage(const cv::Mat &mat);

    FaceDetector *m_detector;
    std::atomic_bool m_faceEnable{false};
    cv::VideoCapture m_cap;

    std::vector<cv::Rect> m_lastFaces;      // 上一帧的检测结果（跳帧复用）
    std::vector<std::vector<cv::Point>> m_lastLandmarks;

    QTimer *m_timer = nullptr;
    int m_frameCounter = 0;
    const int m_detectInterval = 2; // 每2帧检测一次，画面更流畅.越小越流畅，当然性能花费就高
};

#endif // FACECHECKTASK_H
