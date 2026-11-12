#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "verifycodelabel.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>

class loginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit loginWidget(QWidget *parent = nullptr);
    void init_qss();
signals:
};

#endif // LOGINWIDGET_H
