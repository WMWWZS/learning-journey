#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QFile>
#include <QStackedLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
class MonitorWidget : public QWidget
{
    Q_OBJECT
private:
    QStackedLayout* video_layout;
    QHBoxLayout* main_layout;
    QWidget* W_right, *W_left; //错误，在头文件new对象
    QPushButton* btn_channel1,* btn_channel4;
    QLabel *lab_vedeo1,*lab_vedeo[4];
public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    void init_leftWidget();
    void init_rightWidget();
    void init_qss();
    void init_connect();
public slots:
    void switch_to_channel1();
    void switch_to_channel4();
};

#endif // MONITORWIDGET_H
