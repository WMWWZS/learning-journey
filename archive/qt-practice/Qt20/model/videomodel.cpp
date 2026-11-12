#include "videomodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

VideoModel::VideoModel(QObject *parent) : QSqlQueryModel(parent) {}

void VideoModel::getPageModel(int page, int pageSize)
{
    int offset = (page - 1) * pageSize;  //page是2，那就是跳10页，也就是目标11页
    QString sql = "SELECT video_name, start_time, end_time, file_size "
                  "FROM video ORDER BY id DESC LIMIT ?, ?";

    QSqlQuery query(DbConn::getInstance()->getDb());
    query.prepare(sql);
    query.addBindValue(offset);
    query.addBindValue(pageSize);
    if (!query.exec())
        qDebug() << "分页查询失败:" << query.lastError().text();

    this->setQuery(query);
    this->setHeaderData(0, Qt::Horizontal, "视频文件名");
    this->setHeaderData(1, Qt::Horizontal, "开始时间");
    this->setHeaderData(2, Qt::Horizontal, "结束时间");
    this->setHeaderData(3, Qt::Horizontal, "大小(Byte)");
}

int VideoModel::totalCount()   //获取视频总数
{
    QSqlQuery q(DbConn::getInstance()->getDb());
    q.exec("SELECT COUNT(*) FROM video");
    return q.next() ? q.value(0).toInt() : 0;  //移动到那一行结果。成功就取第0列，转成int数字返回
}

int VideoModel::pageCount(int pageSize)   //对视频总数进行分页
{
    int total = totalCount();
    return (total == 0 || pageSize <= 0) ? 1 : (total + pageSize - 1) / pageSize; //算出每页显示多少页
}

bool VideoModel::inser_video(Video &video)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    if(!db.isOpen())
    {
        qDebug() << "数据库连接失败";
        return false;
    }
    QString sql = "insert into video(video_name,video_path,channel_id,create_time)"
                  "values(:video_name,:video_path,:channel_id,:create_time)";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":video_name", video.getVideo_name());
    query.bindValue(":video_path", video.getVideo_path());
    query.bindValue(":channel_id", video.getVideo_id());
    query.bindValue(":create_time",video.getCreate_time());
    if(query.exec())
    {
        if(query.numRowsAffected() > 0)
        {
            return true;
        }
    }
    qDebug() << query.lastError();
    return false;
}

void VideoModel::getFilteredPage(const VideoFilter &filter, int page, int pageSize)
{
    if(page < 1)
    {
        page = 1;
    }
    const int offset = (page - 1) * pageSize; //跳记录公式

    QString sql = "select id, video_name, channel_id, start_time, end_time, "
                  "file_size, video_path from video WHERE 1=1 ";
    QSqlQuery query(DbConn::getInstance()->getDb());
    query.prepare(sql);

    if (filter.channelId > 0)
    {
        sql += "and channel_id = :ch ";
    }
    if (!filter.from.isEmpty())
    {
        sql += "and start_time >= :from ";
    }
    if (!filter.to.isEmpty())
    {
        sql += "and start_time < :to ";
    }                                                               //跳过offset条，取size条
    sql += "order by start_time DESC, id DESC limit :offset, :size"; //先按开始时间倒序（新的在前），时间相同再按id倒序
    query.bindValue(":offset", offset);
    query.bindValue(":size", pageSize);

    if (!query.exec())
    {
        qDebug() << "回放检索失败:" << query.lastError().text();
    }
    this->setHeaderData(0, Qt::Horizontal, "ID");
    this->setHeaderData(1, Qt::Horizontal, "视频文件");
    this->setHeaderData(2, Qt::Horizontal, "通道");
    this->setHeaderData(3, Qt::Horizontal, "开始时间");
    this->setHeaderData(4, Qt::Horizontal, "结束时间");
    this->setHeaderData(5, Qt::Horizontal, "大小(Byte)");
}

int VideoModel::filteredCount(const VideoFilter &filter)
{
    QString sql = "select count(*) from video where 1=1";
    if(filter.channelId > 0)
    {
        sql += "and chnnel_id = :ch";
    }
    if(!filter.from.isEmpty())
    {

        sql += "and start_time >= :from";
    }
    if(!filter.to.isEmpty())
    {
        sql += "and start_time >= :to";
    }
    QSqlQuery query(DbConn::getInstance()->getDb());
    query.prepare(sql);
    if (filter.channelId > 0)
    {
        query.bindValue(":ch", filter.channelId);
    }
    if (!filter.from.isEmpty())
    {
        query.bindValue(":from", filter.from);
    }
    if (!filter.to.isEmpty())
    {
        query.bindValue(":to", filter.to);
    }
    if (!query.exec() || !query.next())
    {
        return 0;  //没成功就是0条
    }
    return query.value(0).toInt(); //成功了，也要将QVariant类转化成对应的类型
}



