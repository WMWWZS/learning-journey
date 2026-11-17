#ifndef EVENTSERVICE_H
#define EVENTSERVICE_H

#include <QObject>

class EventService : public QObject
{
    Q_OBJECT
public:
    explicit EventService(QObject *parent = nullptr);

signals:
};

#endif // EVENTSERVICE_H
