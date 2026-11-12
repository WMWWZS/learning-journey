#ifndef BASETASK_H
#define BASETASK_H
#include <iostream>
#include <string.h>
using namespace std;

class BaseTask
{
protected:
    void* data;
public:
    BaseTask(void* data, size_t len);
    virtual ~BaseTask();
    virtual void service() = 0;  //没有基类实现体，强制所有派生类必须重写该函数
};

#endif // BASKTASK_H
