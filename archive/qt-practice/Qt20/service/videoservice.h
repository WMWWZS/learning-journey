#ifndef VIDEOSERVICE_H
#define VIDEOSERVICE_H

#include <QObject>
#include <QSqlQueryModel>
#include "../model/videomodel.h"

class VideoService : public QObject
{
    Q_OBJECT

public:
    explicit VideoService(QObject *parent = nullptr);
    QSqlQueryModel* getVideoPage(int page, int pageSize);
    int getTotalCount();
    int getPageCount(int pageSize);
    QString getVideoPath(const QString &videoName);

    void getFilteredPage(const VideoFilter &filter, int page, int pageSize);
    //当前条件下的总条数（算总页数用）
    int filteredCount(const VideoFilter &filter);
    //同样条件下的总页数
    QSqlQueryModel* getFilteredPage(const VideoFilter &filter, int page, int pageSize);
private:
    VideoModel *m_model;
};

#endif
