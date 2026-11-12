#include "worker.h"


Worker::Worker(QObject *parent) : QObject{parent}
{

}

void Worker::workLoop()
{
    while(true)
    {
        QMutexLocker locker(&m_mutex);
        if(m_stop)
        {
            qDebug() << "子线程退出";
            break;
        }
        //如果暂停，阻塞休眠，释放CPU
        if(m_pause)
        {
            qDebug() << "子线程进入暂停休眠";
            m_cond.wait(&m_mutex);  //释放锁，休眠，等待唤醒
            continue;
        }

        qDebug()<<"子线程正在干活";
        QThread::sleep(1);
    }
}

void Worker::pauseThread()
{
    QMutexLocker locker(&m_mutex);  //防止数据竞争 同一个共享变量，所有读 所有写，全部都要用同一把互斥锁包起来
    m_pause = true;
}

void Worker::resumeThread()
{
    QMutexLocker locker(&m_mutex);
    m_pause = false;
    m_cond.wakeOne(); //唤醒等待的子线程
}

void Worker::stopThread()
{
    QMutexLocker locker(&m_mutex);
    m_stop = true;
    m_pause = false;
    m_cond.wakeAll();
}
