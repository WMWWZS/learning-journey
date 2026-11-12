#include "regtask.h"
#include "cdata.h"
#include "user.h"

RegTask::RegTask(void *data, size_t len) : BaseTask(data, len)
{

}

RegTask::~RegTask()
{

}

void RegTask::service()
{
    User* user = (User*)this->data;
    cout << user->getUserName() << "  " << user->getPassword() << endl;
    CData::userArr[CData::userSize] = *user;
    CData::userSize++;
    //任务：新增用户写入文件

}
