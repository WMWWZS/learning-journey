#ifndef WORKER_H
#define WORKER_H

#include <QMutex>
#include <QObject>
#include <QWaitCondition>
#include <QDebug>
#include <QThread>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    volatile bool m_stop = false;
    volatile bool m_pause = false;
    QMutex m_mutex;
    QWaitCondition m_cond;
signals:
public slots:
    void workLoop();
    void pauseThread();
    void resumeThread();
    void stopThread();

};

#endif // WORKER_H
