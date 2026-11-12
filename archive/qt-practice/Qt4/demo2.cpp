#include "demo2.h"

demo2::demo2(QWidget *parent) : QWidget{parent}
{

    this->setWindowTitle("为QLabel添加点击事件的两种方式");
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(10);

    main_layout->addStretch();   //吃掉垂直方向剩余高度

    MyLabel* lab = new MyLabel;
    lab->setText("自定义按钮");
    lab->setStyleSheet("background-color: yellow");
    connect(lab, SIGNAL(clicked()), this, SLOT(MyLabel_handle()));
    main_layout->addWidget(lab);

    lab2 = new MyLabel;
    lab2->setText("自定义按钮");
    lab2->setStyleSheet("background-color: blue");
    lab2->installEventFilter(this);   //**lab2 产生的所有事件，先丢给 this (demo2 对象) 的 eventFilter 函数过一遍**。把过滤器挂载上去。
    main_layout->addWidget(lab2);
    main_layout->addStretch();
}

bool demo2::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == lab2 && event->type() == QEvent::MouseButtonPress)
    {
        QMessageBox::information(this, "测试", "自定义按钮1");
        return true;   //事件拦截，吃掉，原始控件不再收到该事件
    }
    return QObject::eventFilter(watched, event);   //放行，事件继续交给原来控件处理
}

void demo2::MyLabel_handle()
{
    QMessageBox::information(this, "测试", "自定义按钮");
}