#include "cameratask.h"

CameraTask::CameraTask(QObject *parent) : QObject{parent}
{



}

int CameraTask::getWidth() const
{
    return width;
}

int CameraTask::getHeight() const
{
    return height;
}

int CameraTask::getFps() const
{
    return fps;
}

void CameraTask::open_camera()
{
    // 获取摄像头
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
        qDebug() << "摄像头打开失败" ;
        return;
    }

    //获取摄像头的一些参数
    width = cap.get(CAP_PROP_FRAME_WIDTH);
    height = cap.get(CAP_PROP_FRAME_HEIGHT);
    fps = cap.get(CAP_PROP_FPS);

    qDebug() << "视频的分辨率:" << width << "*" << height << "帧率:" << fps;
    Mat videoFrame;
    while(cap.read(videoFrame ) && is_stop != true) //读取摄像头 帧存进videoFrame
    {
        emit sig_videoFrame(videoFrame);
    }
    cap.release();
}


void CameraTask::stop_camera()
{
    this->is_stop = true;
}
