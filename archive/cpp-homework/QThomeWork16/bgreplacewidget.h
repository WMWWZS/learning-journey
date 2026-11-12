#ifndef BGREPLACEWIDGET_H
#define BGREPLACEWIDGET_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QPushButton>

class BgReplaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BgReplaceWidget(QWidget *parent = nullptr);
    QImage cvMat2QImage(const cv::Mat &mat);
private:
    cv::Mat m_frontMat;
    cv::Mat m_backMat;

    QLabel* m_labelShow;
    QPushButton* m_btnloadfront;
    QPushButton* m_btnloadback;
    QPushButton* m_btnRun;
signals:

public slots:
    void slotLoadFrontImg();
    void slotLoadBackImg();
    void slotReplaceBg();
};

#endif // BGREPLACEWIDGET_H
