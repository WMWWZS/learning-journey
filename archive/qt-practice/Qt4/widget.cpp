#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent): QWidget(parent)
{
    this->setWindowTitle("自定义日历控件");
    resize(400,300);
    QVBoxLayout *main_layout =new QVBoxLayout(this);
    setLayout(main_layout);

    Datepicker *datePicker = new Datepicker(this);
    main_layout->addWidget(datePicker);
    main_layout->addStretch();

}
Widget::~Widget()
{
    delete ui;
}
