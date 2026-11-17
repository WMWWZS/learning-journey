#include "userservice.h"
/*

*/

UserService::UserService()
{

}

/*
函数名：login
函数参数：username 用户名，password 用户密码
函数功能：调用模型层Model校验用户名与密码，校验通过后更新该用户的最新登录时间
返回值：true 登录校验成功且登录时间更新成功 false 账号密码错误，或更新登录时间失败
*/
bool UserService::login(QString username, QString password)
{
    int userId = 0;
    userModel.printAllUser();
    bool result = userModel.QueryUserAndpwd(username, password, userId);
    if(result)
    {
        qDebug() << "登录校验真";
        int row = userModel.updateLoginTime(userId);
        if(row > 0)
        {
           return true;
        }
    }
    qDebug() << "登录校验假";
    return false;
}
