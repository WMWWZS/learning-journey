#ifndef PRODUCER_H
#define PRODUCER_H

#include <QObject>
#include <QThread>
#include <QDebug>

class Producer : public QObject
{
    Q_OBJECT
public:
    explicit Producer(QObject *parent = nullptr);
signals:
    void sendData(int val);
public slots:
    void produce();
signals:
};

#endif // PRODUCER_H
