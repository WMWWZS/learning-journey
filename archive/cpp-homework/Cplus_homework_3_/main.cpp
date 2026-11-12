#include <iostream>
#include "myfstream.h"
#include "usermodel.h"
using namespace std;

int main()
{
    //1.创建对象
    UserModel UM;
    string name;
    string password;
    //2.打开文件(有则开，无则创) ifstream只读文件操作(磁盘文件->程序内存) ofstream只写文件操作(程序内存 ->磁盘文件) fstream两个都行
    cout << "登录————注册_test" <<endl;

    cout << "注册测试" << endl;
    cout << "请输入name：" << endl;
    cin >> name;

    cout << "请输入password：" << endl;
    cin >> password;
    UM.regiserUser(name, password);

    cout << "登录测试" << endl;
    cout << "请输入name：" << endl;
    cin >> name;

    cout << "请输入password：" << endl;
    cin >> password;
    UM.loginUser(name, password);

    return 0;
}
