#ifndef USERMODEL_H
#define USERMODEL_H
#include "user.h"
class UserModel   //userData.txt用户信息在这
{
protected:
    void* data;
public:
    UserModel(void* data, size_t len);
    virtual void service() = 0;
    virtual~UserModel();
};

#endif // USERMODEL_H
