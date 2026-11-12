#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H

#include <QSqlQueryModel>
#include "util/dbconn.h"
#include "model/video.h"



struct VideoFilter
{
    int channelId = 0;   //0 = 全部通道
    QString from;            //yyyy-MM-dd HH:mm:s
    QString to;
};
//接对接QTableView表格控件的模型类，属于Model/View框架
class VideoModel : public QSqlQueryModel {
    Q_OBJECT
public:
    explicit VideoModel(QObject *parent = nullptr);

    void getPageModel(int page, int pageSize);
    int totalCount();
    int pageCount(int pageSize);
    bool inser_video(Video& video);

    //按通道+时间区间检索
    void getFilteredPage(const VideoFilter &filter, int page, int pageSize);
    //当前条件下的总条数（算总页数用）
    int filteredCount(const VideoFilter &filter);
};

#endif
