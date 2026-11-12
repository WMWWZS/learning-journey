#ifndef WORKTASK_H
#define WORKTASK_H

#include <QObject>
#include <Qdebug>
#include <QThread>

class WorkTask : public QObject
{
    Q_OBJECT
public:
    explicit WorkTask(QObject *parent = nullptr);
signals:
public:
    void doWork();
};

#endif // WORKTASK_H
