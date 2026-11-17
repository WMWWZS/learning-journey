#ifndef REGWIDGET_H
#define REGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "verifycodelabel.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>

class RegWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegWidget(QWidget *parent = nullptr);
    void init_qss();

signals:
};

#endif // REGWIDGET_H
