#ifndef CameraWidget_H
#define CameraWidget_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <opencv2/opencv.hpp>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <cameratask.h>
#include <QDateTime>
#include <QThread>
#include <atomic>

using namespace cv;

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QWidget *parent = nullptr);
    QImage cvMat2QImage(const cv::Mat &mat);
private:
    String path_name;
    std::atomic<bool> is_record;
    CameraTask *cameraTask;
    QLabel *lab_show;
    QPushButton *btn_on, *btn_off, *btn_record;
    VideoWriter *videoWriter;

public slots:
    void open_camera();
    void close_camera();
    void start_record();
    void show_frame(Mat mat);
signals:
};

#endif // CameraWidget_H
