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
#include "monitorwidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout* top_layout = nullptr;
public:
    explicit MainWidget(QWidget *parent = nullptr);

    void init_top_widget();
    void init_qss();
};

#endif // MAINWIDGET_H