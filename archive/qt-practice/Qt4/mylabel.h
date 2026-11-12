#ifndef MYLABEL_H
#define MYLABEL_H

#include <QWidget>
#include <QLabel>
class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent* event);
signals:
    void clicked();
};

#endif // MYLABEL_H
