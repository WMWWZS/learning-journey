#ifndef FACECHECK_H
#define FACECHECK_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <facechecktask.h>

class FaceCheck : public QWidget
{
    Q_OBJECT
public:
    explicit FaceCheck(QWidget *parent = nullptr);

    QImage vcvMat2QImage(const cv::Mat &mat);
private:
    QCheckBox *m_checkFace = nullptr;   // 人脸检测开关
    QLabel *m_labelShow = nullptr;   //画面
    QThread *m_thread = nullptr;   // 工作线程
    FaceCheckTask *m_task = nullptr;   // 工作线程里的对象
signals:

public slots:
    void onFrameReady(const QImage &image);  //显示预备
    void onError(const QString &message);   //错误显示
};
#endif // FACECHECK_H
