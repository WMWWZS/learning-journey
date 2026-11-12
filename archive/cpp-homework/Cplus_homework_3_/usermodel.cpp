#include "usermodel.h"
#include "user.h"
#include <iostream>
#include <fstream>
#include "myfstream.h"
using namespace std;
User UserModel::user[10];  //这里注意

bool UserModel::regiserUser(string UserName, string UserPassword)  //注册
{
    if(User::UserNum >= 10)
    {
        cout << "用户池已满" << endl;
        return false;
    }
    else
    {
        user[User::UserNum] = User(UserName, UserPassword);  //临时对象，我存的的临时对象的数据，而不是本体，这里我可以深入思考？它的内存布局？

        Myfstream outFile("userData.txt");
        if(outFile.is_open())
        {
            //用户名 空格 密码
            /*outFile << UserName << " " << UserPassword << endl;*/ //<<运算符会自动调用 string的转换机制，把内存里的字符串内容变成一串字符，写入文件
            outFile.writeToFile(UserName);
            outFile.writeToFile(UserPassword);
            outFile.close(); //刷新缓冲区，立即写入
            cout << "文件写入成功！" << endl;
            cout << "注册成功" << endl;
            return true;
        }
        else
        {
            cout << "文件打开失败，无法保存用户" << endl;
            return false;
        }
    }
}

bool UserModel::loginUser(string UserName, string UserPassword)  //根据文件内容登录
{
    Myfstream inFile("userData.txt");
    string name, pwd;
    while(inFile >> name >> pwd)
    {
        if(name == UserName && pwd == UserPassword)
        {
            cout << "登录成功" << endl;
            inFile.close();
            return true;
        }
    }
    inFile.close();
    cout << "账号或密码错误" << endl;
    return false;
}

UserModel::UserModel()
{
    Myfstream inFile;
    string name;
    string password;
    while(inFile >> name >> password)
    {
        if(User::UserNum >= 10)
        {
            cout << "用户数组已满！" << endl;
            break;
        }
        else
        {
            user[User::UserNum] = User(name, password);
            User::UserNum++;
        }
    }
}










