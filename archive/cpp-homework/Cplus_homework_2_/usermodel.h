#ifndef USERMODEL_H
#define USERMODEL_H
#include "user.h"
class UserModel
{
public:
    static User user[10];
    bool regiserUser(string UserName, string UserPassword);
    bool loginUser(string UserName, string UserPassword);
    UserModel();
};

#endif // USERMODEL_H
