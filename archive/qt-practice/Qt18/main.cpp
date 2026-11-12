#include "widget.h"
#include "mythread.h"
#include "worktask.h"
#include "sellthread.h"
#include "worker.h"
#include "consumer.h"
#include "producer.h"
#include "printtask.h"
#include <QApplication>

/*
观点1：
只有run()运行在子线程；禁止手动调用run()，必须start()启动线程
*/
void test01()
{
    MyThread *mt = new MyThread;
    mt->start();  //自动调用run()
    mt->wait();
    mt->run();  //错误！直接调用run，代码跑在主线程  主线程 ID 和子线程 ID 数字不一样
    exit(1);
}  //run()不要手动调用！调用start()操作系统分配子线程

/*
观点2：
1.业务类普通 QObject，不继承 QThread；moveToThrea移动对象；started信号触发槽
2.WorkTask不要设置父对象！moveToThread有父对象移动会失败
3.doWork是槽函数，由started信号触发，才运行在子线程
如果我直接写 task->doWork()，开线程是没有意义的
*/

void test02()
{
    WorkTask* wt = new WorkTask;
    QThread* workQthread = new QThread;


    //将业务对象移到子线程
    wt->moveToThread(workQthread);

    //线程started信号触发doWork槽函数
    QObject::connect(workQthread, &QThread::started, wt, &WorkTask::doWork);

    //线程结束自动释放内存
    QObject::connect(workQthread, &QThread::finished, wt, &WorkTask::deleteLater);
    QObject::connect(workQthread, &QThread::started, wt, &QThread::deleteLater);

    workQthread->start();
}

/*
观点3：
1.多个线程同时读写同一个共享变量，一定会产生竞态条件（竞争），结果错乱
2.卖票例子直观：不加锁，两个线程同时读到tickets=1，都执行减 1，票数变成负数
3.互斥锁作用：保护一段临界区代码，同一时刻只允许一个线程进入临界区访问共享资源
*/
void test03()
{
    SellThread* st1 = new SellThread;
    SellThread* st2 = new SellThread;

    st1->start();
    st2->start();

    st1->wait();
    st2->wait();
}

/*
观点4：
1. 线程暂停，绝对不要写 while (m_pause); 空循环等待
空循环会不停轮询判断，CPU 直接拉满 100%，浪费资源
2. QWaitCondition::wait(&mutex)：线程释放锁，进入休眠阻塞，不占用 CPU；等到wakeOne()唤醒之后，再重新拿到锁继续执行。
3. 典型场景：视频播放器暂停 / 播放。暂停时子线程休眠，不消耗 CPU；点击播放唤醒线程继续解码。
4. volatile关键字：防止编译器优化，每次强制从内存读取最新变量，而不是读取寄存器缓存的旧值。
*/
void test04()
{
    Worker* worker = new Worker;
    QThread* sub = new QThread;

    worker->moveToThread(sub);
    QObject::connect(sub, &QThread::started, worker, &Worker::workLoop);  //workLoop 进入子线程执行
    sub->start();

    qDebug()<<"3秒后调用pauseThread，子线程暂停";
    QThread::sleep(3);
    worker->pauseThread(); //出了{}锁自动释放，由于 m_pause为真 子线程休眠了

    qDebug()<<"再3秒，resumeThread唤醒继续运行";
    QThread::sleep(3);
    worker->resumeThread();

    qDebug()<<"再过3秒，停止线程";
    QThread::sleep(3);
    worker->stopThread();

    sub->wait();
    delete worker;
    delete sub;
    exit(1);
}

/*
观点5：
1.Qt 跨线程首：信号槽通信，自动线程安全，不需要手动加锁传递数据。
底层原理：跨线程信号会把数据打包成事件，投递到接收方线程的事件循环队列。
2.生产者消费者模型：
生产者（子线程）：负责耗时工作，生产数据（案例里的数字；播放器里就是 cv::Mat 帧）
消费者（主线程）：接收数据，只做 UI 渲染，主线程不做耗时任务
3.铁律：子线程严禁操作任何 UI 控件。所有界面更新，全部通过信号槽把数据传给主线程，由主线程操作控件。
*/
void test05()
{
    qDebug()<<"main主线程ID:"<<QThread::currentThreadId();
    Producer* prod = new Producer;
    QThread* subThread = new QThread;
    prod->moveToThread(subThread);

    Consumer* cons = new Consumer; //主线程的数据消费者

    QObject::connect(subThread, &QThread::started, prod, &Producer::produce);
    QObject::connect(prod, &Producer::sendData, cons, &Consumer::recvData);

    QObject::connect(subThread, &QThread::finished, prod, &QObject::deleteLater);   // produce()完成触发finished信号
    QObject::connect(subThread, &QThread::finished, subThread, &QObject::deleteLater);
    QObject::connect(subThread, &QThread::finished, cons, &QObject::deleteLater);


    subThread->start();


}

/*
观点6：
QSemaphore（信号量）是增强版互斥锁，可以设置 N 份资源，
用来限制同一时间最多 N 个线程并发访问有限资源。acquire 申请资源，
资源不够线程阻塞；release 释放资源，唤醒等待线程。
*/
void test06()
{
    int threadCount = 5;
    PrintTask* tasks[threadCount];
    for(int i = 0; i < threadCount; i++)
    {
        tasks[i] = new PrintTask;
        tasks[i]->start(); //run（）
    }

    //等待所有线程执行完毕
    for(int i = 0; i < threadCount; i++)
    {
        tasks[i]->wait();
        delete tasks[i];
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    test06();
    return QApplication::exec();
}
