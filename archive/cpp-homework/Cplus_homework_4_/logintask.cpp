#include "logintask.h"
#include "cdata.h"
#include "user.h"
#include "myfstream.h"

LoginTask::LoginTask(void *data, size_t len) : UserModel(data, len)  //构造 loginTask 对象的时候，先调用父类 BaseTask 的构造函数，把 data、len 传给父类
{

}

LoginTask::~LoginTask()
{

}

void LoginTask::service()
{
    Myfstream inFile("userData.txt");
    User* user = (User*)this->data;
    cout << "当前用户："<<user->getUserName() << endl;
    string name, pwd;
    while(inFile >> name >> pwd)
    {
        if(name == user->getUserName() && pwd == user->getUserPassword())
        {
            cout << "登录成功" << endl;
            inFile.close();
            return;
        }
    }
    inFile.close();
    cout << "账号或密码错误" << endl;
    return;
}
