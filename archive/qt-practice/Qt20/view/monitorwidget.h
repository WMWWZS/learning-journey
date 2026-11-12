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
#include <QHeaderView>
#include <QSqlError>
#include <QVector>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <opencv2/opencv.hpp>
#include "../util/dbconn.h"
#include "../service/monitortask.h"
#include "../service/recordtask.h"
using namespace cv;

class MonitorWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel *m_labels[4] = {nullptr, nullptr, nullptr, nullptr};  //四宫格显示
    QVector<MonitorTask*> m_tasks;     //4个采集任务（只是方便统一 stop）
    QVector<QThread*> m_threads;   //4个采集线程
    RecordTask *m_recordTask = nullptr;
    QThread *m_recordThread = nullptr;

    QImage img;
    QStackedLayout* video_layout;
    QHBoxLayout* main_layout;
    QWidget* W_right, *W_left;
    QPushButton* btn_channel1,* btn_channel4;
    QLabel *lab_vedeo1,*lab_vedeo[4];
public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    ~MonitorWidget() override;
    QImage MatToQImage(const Mat& frame);
    void stop_all();
    void init_leftWidget();
    void init_rightWidget();
    void init_qss();
    void init_connect();
public slots:
    void switch_to_channel1();
    void switch_to_channel4();
    void start_palyer();
};

#endif // MONITORWIDGET_H
