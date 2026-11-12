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
#include "monitorwidget.h"
#include "loginwidget.h"
#include "datepicker.h"

class MainWidget : public QWidget
{
    Q_OBJECT
private:
    QTimer* m_timer;
    QLabel* lab_time;
    QPushButton* btn_login;
    QHBoxLayout* top_layout;
    loginWidget* wid_login;
    QLabel* lab_login;
    Datepicker* datepicker;
public:
    explicit MainWidget(QWidget *parent = nullptr);
    void init_showtime();
    void init_connect();
    void init_top_widget();
    void init_qss();
public slots:
    void update_time();
    void to_login_win();
    void back_to_mainWin(QString username);
};

#endif // MAINWIDGET_H