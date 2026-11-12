#ifndef REGTASK_H
#define REGTASK_H
#include "basetask.h"
class RegTask : public BaseTask
{
public:
    RegTask(void* data, size_t len);
    ~RegTask();
    void service();
};

#endif // REGTASK_H
