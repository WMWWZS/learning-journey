#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QFile>
class MonitorWidget : public QWidget
{
private:
    QWidget* W_left = new QWidget;
    Q_OBJECT
public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    void init_leftWidget();
    void init_qss();
signals:
};

#endif // MONITORWIDGET_H
