#include "video.h"

Video::Video() {}

int Video::getVideo_id() const
{
    return video_id;
}

void Video::setVideo_id(int newVideo_id)
{
    video_id = newVideo_id;
}

void Video::setVideo_name(const QString &newVideo_name)
{
    video_name = newVideo_name;
}

QString Video::getVideo_name() const
{
    return video_name;
}

QString Video::getVideo_path() const
{
    return video_path;
}

void Video::setVideo_path(const QString &newVideo_path)
{
    video_path = newVideo_path;
}

void Video::setCreate_time(const QString &newCreate_time)
{
    create_time = newCreate_time;
}

QString Video::getCreate_time() const
{
    return create_time;
}
