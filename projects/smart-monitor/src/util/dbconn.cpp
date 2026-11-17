#include "dbconn.h"
DbConn* DbConn::s_instance = nullptr;
QSqlDatabase DbConn::getDb() const
{
    return db;
}

void DbConn::releaseInstance()
{
    if(DbConn::s_instance != nullptr)
    {
        delete DbConn::s_instance;
        DbConn::s_instance = nullptr;
    }
}

DbConn *DbConn::getInstance()
{
    if(s_instance == nullptr)
    {
        s_instance = new DbConn;
        return s_instance;
    }
    else
    {
        return s_instance;
    }
}

DbConn::DbConn()
{
    //1.注册MySQL驱动
    db = QSqlDatabase::addDatabase("QMYSQL");
    //2.设置数据库连接参数
    db.setHostName("localhost");
    db.setPort(3306);
    db.setPassword("123456");
    db.setUserName("root");
    db.setDatabaseName("monitor");

    qDebug() << "支持的驱动：" << QSqlDatabase::drivers();
    if(db.open())
    {
        qDebug() << "数据库连接成功" ;
    }
    else
    {
        qDebug() << "数据库连接失败：" << db.lastError().text();
    }
}

DbConn::~DbConn()
{
    qDebug() << "释放数据库连接";
    if(db.isOpen())
    {
        db.close();  //断开数据
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);  //移除默认连接
    }
}


// 实现MD5加密
QString DbConn::getMd5(const QString &str)
{
    // 1、将字符串转为UTF-8编码的字节数组（避免中文乱码）
    QByteArray byteArr = str.toUtf8();
    // 2、计算MD5哈希值
    QByteArray md5Arr = QCryptographicHash::hash(byteArr, QCryptographicHash::Md5);
    // 3、转为十六进制的字符串
    QString md5Str = md5Arr.toHex();
    return md5Str;
}










