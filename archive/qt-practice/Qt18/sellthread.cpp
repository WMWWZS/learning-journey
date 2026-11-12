#include "sellthread.h"

int SellThread::tickets = 10; //共享全局票数
QMutex SellThread::g_mutex;
SellThread::SellThread(QObject *parent) : QThread{parent}
{}


void SellThread::run()
{
    while(true)
    {
        { // 局部作用域，仅保护共享数据
            QMutexLocker locker(&g_mutex);
            if(tickets <= 0) {
                qDebug() << "票已卖完";
                break;
            }
            tickets--;
            qDebug() << currentThreadId() << "卖出，剩余:" << tickets;
        } // 锁立即释放

        QThread::msleep(1); //耗时操作放锁外，不阻塞其他线程
    }
}

//这种导致b抢不到买票的机会

// QMutexLocker locker(&g_mutex); while(){}里的锁
// if(tickets > 0)
// {
//     QThread::msleep(1);   //线程A 自己休眠 1ms，但锁还攥在手里
//     tickets--;
//     qDebug() << currentThreadId() << "卖出一张，剩余：" << tickets;
// }
// else
// {
//     qDebug() << "票已卖完" ;
//     break;
// }
