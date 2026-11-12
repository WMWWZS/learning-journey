#ifndef DEMO_H
#define DEMO_H

#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>

class demo : public QWidget
{
    Q_OBJECT
public:
    explicit demo(QWidget *parent = nullptr);
protected:
    void closeEvent(QCloseEvent* event);
    void paintEvent(QPaintEvent* event);
signals:
signals:

public slots:
};

#endif // DEMO_H
