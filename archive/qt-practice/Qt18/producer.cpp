#include "producer.h"

Producer::Producer(QObject *parent) : QObject{parent}
{

}

void Producer::produce()
{
    for(int i = 0; i < 5; i++)
    {
        qDebug() << "[生产者子线程]生产的数据" << i << "线程ID:"  << QThread::currentThreadId();
        emit sendData(i); //发射信号，把数据发送出去
        QThread::sleep(1);
    }
}
