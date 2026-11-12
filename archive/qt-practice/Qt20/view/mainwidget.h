#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <QStackedLayout>
#include "monitorwidget.h"
#include "datepicker.h"
#include "videowidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT
private:
    QStackedLayout *m_pages = nullptr;
    MonitorWidget* m_monitorPage = nullptr;  //index 0：监控主界面
    VideoWidget* m_videoPage = nullptr;  //index 1：普通视频回看
    QWidget* m_zhuizongPage = nullptr; //index 2：移动侦测回看（占位）
    QWidget* m_logPage = nullptr;//index 3：日志（占位）
    QWidget* m_settingPage  = nullptr;// index 4：系统设置（占位）

    QPushButton* btn_jiankong, *btn_review, *btn_setting, *btn_zhuizong, *btn_log;
    QTimer* m_timer;
    QLabel* lab_time;
    QPushButton* btn_login;
    QHBoxLayout* top_layout;
    QLabel* lab_login;
public:
    explicit MainWidget(QWidget *parent = nullptr);
    void showUsername(const QString username);
    void init_showtime();
    void init_connect();
    void init_top_widget();
    void init_qss();
public slots:
    void update_time();
    void to_login_win();
};

#endif // MAINWIDGET_H
