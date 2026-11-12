#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "../util/dbconn.h"
#include "../model/usermodel.h"
#include <QString>
class UserService
{
public:
    UserModel userModel;
    UserService();
    bool login(QString username, QString password);
private:

};

#endif // USERSERVICE_H
