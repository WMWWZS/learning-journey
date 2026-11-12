#include "basetask.h"

BaseTask::BaseTask(void* data, size_t len)
{
    this->data = new char[len];
    memset(this->data, 0, len);
    memcpy(this->data ,data, len);
}

BaseTask::~BaseTask()
{
    if(data != NULL)
    {
        delete [](char*)this->data;    //delete[] 不能直接操作 void*
        this->data = nullptr;
        cout << "已释放data单元数据" << endl;
    }
}
