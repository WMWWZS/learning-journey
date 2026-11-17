#ifndef DETECTIONTASK_H
#define DETECTIONTASK_H

#include <QObject>

class DetectionTask : public QObject
{
    Q_OBJECT
public:
    explicit DetectionTask(QObject *parent = nullptr);

signals:
};

#endif // DETECTIONTASK_H
