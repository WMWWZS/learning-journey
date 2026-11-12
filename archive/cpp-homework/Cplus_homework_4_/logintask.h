#ifndef LOGINTASK_H
#define LOGINTASK_H
#include "usermodel.h"


class LoginTask : public UserModel
{
public:
    LoginTask(void* data, size_t len);
    ~LoginTask();
    void service();
};

#endif // LOGINTASK_H
