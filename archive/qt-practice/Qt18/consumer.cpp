#include "consumer.h"

Consumer::Consumer(QObject *parent)
    : QObject{parent}
{}

void Consumer::recvData(int v)
{
    qDebug() <<"[消费者主线程]收到数据："<<v<<" 线程ID:"<<QThread::currentThreadId();
}
