#ifndef DBCONN_H
#define DBCONN_H
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QCryptographicHash>
#include <QSqlQuery>

class DbConn
{
private:
    QSqlDatabase db;
    static DbConn* s_instance;
    DbConn();
    ~DbConn();
    DbConn(const DbConn&) = delete;
    DbConn& operator=(const DbConn&) = delete;
public:
    QString getMd5(const QString &str);
    QSqlDatabase getDb() const;
    static void releaseInstance();
    static DbConn* getInstance();
};

#endif // DBCONN_H

















