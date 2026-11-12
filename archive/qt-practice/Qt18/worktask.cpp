#include "worktask.h"

WorkTask::WorkTask(QObject *parent)
    : QObject{parent}
{}

void WorkTask::doWork()
{
    qDebug() << "doWork 子线程id：" << QThread::currentThreadId();
    for(int i = 0; i < 5; i++)
    {
        qDebug() << "doWork 任务执行i:" << i;
        QThread::sleep(1);
    }
}
