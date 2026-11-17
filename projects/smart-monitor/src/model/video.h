#ifndef VIDEO_H
#define VIDEO_H
#include <QString>

class Video
{
public:
    Video();


    int getVideo_id() const;
    void setVideo_id(int newVideo_id);

    void setVideo_name(const QString &newVideo_name);
    QString getVideo_name() const;

    QString getVideo_path() const;
    void setVideo_path(const QString &newVideo_path);

    void setCreate_time(const QString &newCreate_time);

    QString getCreate_time() const;

private:
    int video_id;
    QString video_name;
    QString video_path;
    int channel_id;
    QString create_time;
};

#endif // VIDEO_H
