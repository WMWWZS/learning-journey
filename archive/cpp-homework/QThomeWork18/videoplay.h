#ifndef VIDEOPLAY_H
#define VIDEOPLAY_H

#include "videodecoder.h"
#include <opencv2/opencv.hpp>
#include <QWidget>
#include <QTimer>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <QThread>

class VideoPlay : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPlay(QWidget *parent = nullptr);
    QImage cvMat2QImage(const cv::Mat &mat);
public slots:
    void slot_readFrame();
    void slot_playPause();
    void slot_sliderDrag(int frameNo);
    void slot_speedChangedText(int index);
    void slot_recvFrame(const QImage& img);
    void slot_decoderFinished();

signals:
    void requestSeek(int frame); //用于跳转到子线程

private:
    //控件
    QTimer* m_timer;
    QSlider* m_slider;
    QPushButton* m_btn_play;
    QLabel* m_label_show;
    QComboBox* m_combo_speed;

    //视频相关
    cv::VideoCapture m_cap;
    long m_total_frames;
    bool m_is_playing = false;
    double m_fps;

    //线程
    QThread *m_decoder_QThread;
    VideoDecoder* m_decoder;
};

#endif // VIDEOPLAY_H
