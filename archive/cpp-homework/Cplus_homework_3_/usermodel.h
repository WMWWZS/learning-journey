#ifndef USERMODEL_H
#define USERMODEL_H
#include "user.h"
class UserModel   //userData.txt用户信息在这
{
public:
    static User user[10];
    bool regiserUser(string UserName, string UserPassword);
    bool loginUser(string UserName, string UserPassword);
    UserModel();
};

#endif // USERMODEL_H
