#include "logintask.h"
#include "cdata.h"
#include "user.h"

LoginTask::LoginTask(void *data, size_t len) : BaseTask(data, len)  //构造 loginTask 对象的时候，先调用父类 BaseTask 的构造函数，把 data、len 传给父类
{

}

LoginTask::~LoginTask()
{

}

void LoginTask::service()
{
    User* user = (User*)this->data;
    cout << user->getUserName() << "  " << user->getPassword() << endl;
    for(int i = 0; i < CData::userSize; i++)
    {
        User u = CData::userArr[i];
        if(user->getUserName().compare(u.getUserName()) == 0 &&
        user->getPassword().compare(u.getPassword()) == 0)
        {
            cout << "登录成功" << endl;
            return;
        }
    }
    cout << "用户名或者密码错误" << endl;
}
