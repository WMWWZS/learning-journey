#ifndef PRINTTASK_H
#define PRINTTASK_H

#include <QThread>
#include <QSemaphore>

class PrintTask : public QThread
{
public:
    explicit PrintTask(QObject *parent = nullptr);
    //静态信号量，所有线程共享，初始资源3
    static QSemaphore printSemaphore;
protected:
    void run() override;
};

#endif // PRINTTASK_H
