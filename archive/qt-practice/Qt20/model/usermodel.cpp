#include "usermodel.h"

#include <util/dbconn.h>


UserModel::UserModel()
{

}

/*
函数名：QueryUserAndPwd
函数参数：username 用户名，password 明文密码，userId 引用参数，用于带出查询到的用户id
函数功能：根据用户名和密码校验用户，密码经过MD5加密后与数据库对比 查询成功则把用户id存入userId
返回值：true 查询匹配到用户；false 数据库连接失败、SQL执行失败或账号密码不匹配
*/
bool UserModel::QueryUserAndpwd(QString &username, QString &password, int &userId)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    if(!db.isOpen())
    {
        qDebug() << "数据库连接失败：" << db.lastError().text();
        return false;
    }
    QString sql = "select * from admin_user where username = :username and user_password = :user_password";
    //创建查询对象，并且绑定已经打开的数据库连接db
    QSqlQuery query(db);
    //预编译sql语句
    query.prepare(sql);
    query.bindValue(":username", username);
//    query.bindValue(":user_password",DbConn::getInstance()->getMd5(password));
    query.bindValue(":user_password",password);

    if(query.exec())
    {
        if(query.next())   //登录只需要判断有没有一行，while也能用，但没必要，不匹配就会一直读下去
        {
            userId = query.value("id").toInt();
            qDebug() << "查询到用户：id=" << userId << "用户名=" << query.value("username").toString();
            return true;
        }
        return false;
    }
}

/*
函数名：updateLoginTime
参数：用户id
功能：更新登录时间
返回值：
-1 : SQL执行出错
0  : 没有找到该用户，更新0行，SQL 正常执行，但是id不存在，一行都没改动
1  : 更新成功
*/
int UserModel::updateLoginTime(int& userid)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    if(!db.isOpen()){
        qDebug() << "数据库连接失败";
        return -1;
    }
    QString sql = "update admin_user set last_log_time = :logintime where id = :id";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":logintime",QDateTime::currentDateTime());
    query.bindValue(":id",userid);
    if(query.exec())   //定位false 说明sql返回false last_login_time拼写错误
    {
        int affected = query.numRowsAffected();
        qDebug() << "affected rows:" << affected;
        return query.numRowsAffected();
    }
    else
    {
        qDebug() << "更新登录时间失败:"
                 << query.lastError().text()
                 << "driver:" << query.lastError().driverText()
                 << "database:" << query.lastError().databaseText()
                 << "sql:" << sql;
        return -1;
    }
}

void UserModel::printAllUser()
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    if(!db.isOpen())
    {
        qDebug()<<"数据库未打开";
        return;
    }
    QSqlQuery query(db);
    QString sql = "SELECT id,username,user_password,last_log_time,create_time,state FROM admin_user";
    if(query.exec(sql))
    {
        qDebug()<<"======= 全部用户数据 =======";
        while(query.next())
        {
            int id = query.value("id").toInt();
            QString name = query.value("username").toString();
            QString pwd = query.value("user_password").toString();
            QString lastTime = query.value("last_log_time").toString();
            QString createTime = query.value("create_time").toString();
            int state = query.value("state").toInt();
            qDebug()<<"id:"<<id
                    <<" username:"<<name
                    <<" pwd:"<<pwd
                    <<" last_log:"<<lastTime
                    <<" create:"<<createTime
                    <<" state:"<<state;
        }
    }
    else
    {
        qDebug()<<"查询失败："<<query.lastError().text();
    }
}
