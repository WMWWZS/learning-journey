#ifndef DEMO2_H
#define DEMO2_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QEvent>
#include "mylabel.h"
class demo2 : public QWidget
{
    Q_OBJECT
public:
    MyLabel* lab2;
    explicit demo2(QWidget *parent = nullptr);
protected:
    bool eventFilter(QObject* watched, QEvent* event);
signals:

public slots:
    void MyLabel_handle();
};

#endif // DEMO2_H
