#include "monitorwidget.h"

MonitorWidget::MonitorWidget(QWidget *parent) : QWidget{parent}
{
    QHBoxLayout* main_layout = new QHBoxLayout(this);
    main_layout->addWidget(W_left, 1);

    QWidget* W_right = new QWidget;
    main_layout->addWidget(W_right);

    this->init_qss();
    this->init_leftWidget();
}

void MonitorWidget::init_leftWidget()
{
    QVBoxLayout* left_layout = new QVBoxLayout(W_left);

    QLabel* lab_devicelist = new QLabel("设别列表");
    lab_devicelist->setAlignment(Qt::AlignCenter);
    lab_devicelist->setObjectName("devicelist");
    left_layout->addWidget(lab_devicelist);

    QListWidget* device_list = new QListWidget;
    left_layout->addWidget(device_list);
    QListWidgetItem* item1 = new QListWidgetItem(QIcon(":/image/camara.png"),"相机1");
    device_list->addItem(item1);
    QListWidgetItem* item2 = new QListWidgetItem(QIcon(":/image/camara.png"),"相机2");
    device_list->addItem(item2);
    QListWidgetItem* item3 = new QListWidgetItem(QIcon(":/image/camara.png"),"相机3");
    device_list->addItem(item3);

    left_layout->addLayout(left_layout);
    left_layout->addStretch();

}

void MonitorWidget::init_qss()
{
        QFile file(":/qss/Monitor.qss");
        if(file.open(QFile::ReadOnly))
        {
            this->setStyleSheet(file.readAll());
            file.close();
        }
}
