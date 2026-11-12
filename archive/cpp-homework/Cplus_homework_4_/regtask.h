#ifndef REGTASK_H
#define REGTASK_H
#include "usermodel.h"

class RegTask : public UserModel
{
public:
    RegTask(void* data, size_t len);
    ~RegTask();
    void service();
};

#endif // REGTASK_H
