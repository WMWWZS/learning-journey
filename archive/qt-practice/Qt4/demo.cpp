#include "demo.h"

demo::demo(QWidget *parent) : QWidget{parent}
{
    this->setWindowTitle("关闭事件测试");

}

void demo::closeEvent(QCloseEvent *event)
{
    int result = QMessageBox::warning(this, "退出提示", "是否选择退出", QMessageBox::Yes | QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void demo::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap bg(":/image/4.png");
    if(bg.isNull())
    {
        painter.fillRect(rect(), Qt::white);
        return;
    }
    painter.drawPixmap(rect(), bg.scaled(rect().size(), Qt::KeepAspectRatio,Qt::SmoothTransformation));
}
