#include "regtask.h"
#include "cdata.h"
#include "user.h"
#include "myfstream.h"

RegTask::RegTask(void *data, size_t len) : UserModel(data, len)
{

}

RegTask::~RegTask()
{

}

void RegTask::service()
{
    if(CData::userSize >= 10)
    {
        cout << "用户池已满" << endl;
        return;
    }
    //任务：新增用户写入文件
    Myfstream outFile("userData.txt");
    if(outFile.is_open())
    {
        User* user = (User*)this->data;
        //用户名 空格 密码
        /*outFile << UserName << " " << UserPassword << endl;*/ //<<运算符会自动调用 string的转换机制，把内存里的字符串内容变成一串字符，写入文件
        outFile.writeToFile(user->getUserName());
        outFile.writeToFile(user->getUserPassword());
        outFile.close(); //刷新缓冲区，立即写入
        cout << "文件写入成功！" << endl;
        cout << user->getUserName() << "注册成功" << endl;
        CData::userArr[CData::userSize] = *user;
        CData::userSize++;
        return;
    }
    else
    {
        cout << "文件打开失败，无法保存用户" << endl;
        return;
    }
}