#include "recordtask.h"

#include <QCoreApplication>
#include <QDir>  //建目录，拼接文件
#include <QSqlQuery>  //prepare执行
#include <QSqlError>  //数据库错误信息
#include <QFileInfo>  //获取文件大小

RecordTask::RecordTask(int m_segmentSeconds, QObject *parent) : QObject(parent)
    , m_segmentSeconds(m_segmentSeconds > 0 ? m_segmentSeconds : 60)
{
    m_rootDir = QDir(QCoreApplication::applicationDirPath()).filePath("records");  //拼出一个文件夹路径，指向程序所在目录下的records文件夹
    QDir().mkpath(m_rootDir);  //创建文件夹
}

RecordTask::~RecordTask()
{

}

void RecordTask::init()
{
    ensureDb();
}

void RecordTask::save_frame(cv::Mat frame, int channelId, double fps)
{
    if(frame.empty())
    {
        return;
    }

    //这一路还没开文件 那就开一个
    auto it = m_writers.find(channelId);
    if(it == m_writers.end() || !it->writer.isOpened())
    {
        if(!openSegment(channelId, frame,fps))
        {
            return;
        }
    }

    channelWrite &w = m_writers[channelId];
    w.writer.write(frame);
    //写满一段就收尾，下一帧会自动开新段
    if (++w.frameCount >= w.targetFrame)
    {
        closeSegment(channelId);
    }
}

void RecordTask::finish()
{
    const QList<int> keys = m_writers.keys();
    for(int channelId : keys)
    {
        closeSegment(channelId);
    }
}

bool RecordTask::ensureDb()
{
    const QString connName = "record_conn";
    if(QSqlDatabase::contains(connName))  //查重，该连接是否存在
    {
        m_db = QSqlDatabase::database(connName);  //如果存在那么直接连接
    }
    else //不存在就创建
    {
        m_db = QSqlDatabase::addDatabase("QMYSQL", connName);
        m_db.setHostName("localhost");
        m_db.setPort(3306);
        m_db.setUserName("root");
        m_db.setPassword("123456");
        m_db.setDatabaseName("monitor");
    }
    if(!m_db.isOpen() && !m_db.open())
    {
        emit sig_error(QString("录制线程数据库连接失败:%1")
                           .arg(m_db.lastError().text()));
        return false;
    }
    return true;
}

//为某个通道开一个新文件
bool RecordTask::openSegment(int channelId, const cv::Mat &frame, double fps)
{
    channelWrite w;
    w.startTime = QDateTime::currentDateTime();
    w.fps = (fps > 0) ? fps : 30.0;
    w.frameCount = 0;
    w.size = frame.size();
    w.filePath = buildPath(channelId, w.startTime);
    w.targetFrame = static_cast<int>(std::lround(w.fps * m_segmentSeconds));  //目标帧数
    //得到"Record_20260115_143512_ch01.mp4"
    w.fileName = QFileInfo(w.filePath).fileName(); //直接获取当前文件名不带相对路径

    const int fourccH264 = cv::VideoWriter::fourcc('H', '2', '6' , '4');
    //注意string是std::string里的string
    w.writer.open(w.filePath.toStdString(), fourccH264, w.fps, w.size, true);

    if(!w.writer.isOpened())
    {
        emit sig_error(QString("通道%1无法创建视频文件：%2")
                           .arg(channelId)
                           .arg(w.filePath));
        return false;
    }

    qDebug() << "channel：" << channelId << "文件名：" << w.fileName
             << w.size.width << "x" << w.size.height << " " << w.fps << "fps,"
             << w.targetFrame << "frames/segment";

    //存入
    m_writers.insert(channelId, w);
    return true;
}

void RecordTask::closeSegment(int channelId)
{
    auto it = m_writers.find(channelId);
    if(it == m_writers.end())
    {
        return;
    }

    channelWrite &w = it.value();  //找到就取出改通道的对象
    if(w.writer.isOpened())
    {
        w.writer.release();
    }

    if(w.frameCount > 0)  //写过才存进去
    {
        insertRecord(channelId, w);
        emit sig_segment_finished(channelId, w.filePath);
    }

    m_writers.erase(it);
}

bool RecordTask::insertRecord(int channelId, const channelWrite &w)
{
    if(!ensureDb())
    {
        qDebug() << "数据库打开失败";
        return false;
    }
    QSqlQuery query(m_db);  //数据库操作对象

    QString sql = "INSERT INTO video "
                  "(video_name, video_path, channel_id, start_time, end_time, file_size, create_time) "
                  "VALUES "
                  "(:name, :path, :channel, :start, :end, :size, :create)";

    query.prepare(sql);

    const QString appDir = QCoreApplication::applicationDirPath();
    const QString relativePath = QDir(appDir).relativeFilePath(w.filePath);  //转成关于exe路径的相对路径

    query.bindValue(":name", w.fileName);
    query.bindValue(":path", relativePath);
    query.bindValue(":channel", channelId);
    query.bindValue(":start", w.startTime.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":end", w.startTime.addMSecs(
                                static_cast<qint64>(1000.0 * w.frameCount / w.fps)).toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":size", QFileInfo(w.filePath).size());  //获取当前文件的大小
    query.bindValue(":create", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    if(!query.exec())
    {
        emit sig_error(QString("通道%1录像入库失败:%2")
                           .arg(channelId)
                           .arg(query.lastError().text()));
        return false;
    }
    qDebug() << QString(QString("通道%1录像入库成功:")
                           .arg(channelId));
    return true;
}

QString RecordTask::buildPath(int channelId, const QDateTime &time)
{
    const QString day = time.toString("yyyyMMdd");
    const QString sub = QString("ch%1/%2").arg(channelId).arg(day);

    QDir dir(m_rootDir);
    dir.mkpath(sub);   // 目录不存在就创建

    // m_rootDir/sub/name
    const QString name = QString("Record_%1_ch%2.mp4")
                             .arg(time.toString("yyyyMMdd_HHmmss"))
                             .arg(channelId);

    return dir.filePath(sub + "/" + name);
}
