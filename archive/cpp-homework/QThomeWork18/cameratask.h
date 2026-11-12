#ifndef CAMERATASK_H
#define CAMERATASK_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <Qdebug>
#include <QImage>
using namespace cv;

class CameraTask : public QObject
{
    Q_OBJECT
public:
    explicit CameraTask(QObject *parent = nullptr);

    int getWidth() const;

    int getHeight() const;

    int getFps() const;

private:
    bool is_stop {false};
    int width;
    int height;
    int fps;
public slots:
    void open_camera();
    void stop_camera();
signals:
    void sig_videoFrame(Mat frame);

};

#endif // CAMERATASK_H
