#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent)
    : QLabel{parent}
{}

void MyLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
