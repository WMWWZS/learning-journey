#include "printtask.h"
#include <QDebug>
QSemaphore PrintTask::printSemaphore(3);

PrintTask::PrintTask(QObject *parent) : QThread{parent}
{}

void PrintTask::run()
{
    //申请1份资源，资源不足，线程阻塞停在这里
    printSemaphore.acquire();

    //临界区：最多3个线程能同时执行这里
    qDebug() << "线程" << QThread::currentThreadId() << "打印中...";
    QThread::sleep(2); //模拟打印耗时2秒
    qDebug() << "线程" << QThread::currentThreadId() << "打印完成";

    //释放资源，唤醒等待线程
    printSemaphore.release();
}
