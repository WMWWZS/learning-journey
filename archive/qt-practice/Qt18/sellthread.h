#ifndef SELLTHREAD_H
#define SELLTHREAD_H

#include <QThread>
#include <QDebug>
#include <QMutex>

class SellThread : public QThread
{
    static int tickets;
    static QMutex g_mutex; //全局互斥锁
public:
    explicit SellThread(QObject *parent = nullptr);
    void run() override;
};

#endif // SELLTHREAD_H
