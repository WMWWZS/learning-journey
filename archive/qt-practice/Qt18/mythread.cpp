#include "mythread.h"

// run是线程入口，只有这里跑在子线程
void MyThread::run()
{
    qDebug() << "子线程id：" << QThread::currentThreadId();
    for(int i = 0; i < 5; i++)
    {
        qDebug() << "子线程打印i:" << i;
        QThread::sleep(1);
    }
}

MyThread::MyThread() {}
