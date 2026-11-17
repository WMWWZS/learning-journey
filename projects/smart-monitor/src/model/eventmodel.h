#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QObject>

class EventModel : public QObject
{
    Q_OBJECT
public:
    explicit EventModel(QObject *parent = nullptr);

signals:
};

#endif // EVENTMODEL_H
