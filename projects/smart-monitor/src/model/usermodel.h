#ifndef USERMODEL_H
#define USERMODEL_H

#include <QDate>
#include "../util/dbconn.h"
#include <QString>
class UserModel
{
public:
    UserModel();
    bool QueryUserAndpwd(QString& username, QString& password, int& userId);
    int updateLoginTime(int &userid);
    void printAllUser();
};

#endif // USERMODEL_H
