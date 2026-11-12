#include "videoservice.h"
#include <QDir>
#include <QDebug>

VideoService::VideoService(QObject *parent) : QObject(parent)
{
    m_model = new VideoModel(this);
}

QSqlQueryModel* VideoService::getVideoPage(int page, int pageSize)
{
    if (page < 1)
        page = 1;
    m_model->getPageModel(page, pageSize);
    return m_model;
}

int VideoService::getTotalCount()
{
    return m_model->totalCount();
}

int VideoService::getPageCount(int pageSize)
{
    if (pageSize <= 0)
        pageSize = 10;
    return m_model->pageCount(pageSize);
}

QString VideoService::getVideoPath(const QString &videoName)
{
    return QDir::currentPath() + "/videos/" + videoName + ".mp4";
}

void VideoService::getFilteredPage(const VideoFilter &filter, int page, int pageSize)
{

}

int VideoService::filteredCount(const VideoFilter &filter)
{

}

QSqlQueryModel *VideoService::getFilteredPage(const VideoFilter &filter, int page, int pageSize)
{

}
