#ifndef LOGINTASK_H
#define LOGINTASK_H
#include "basetask.h"

class LoginTask : public BaseTask
{
public:
    LoginTask(void* data, size_t len);
    ~LoginTask();
    void service();
};

#endif // LOGINTASK_H
