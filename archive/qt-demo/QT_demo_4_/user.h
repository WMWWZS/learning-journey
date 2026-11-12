#ifndef USER_H
#define USER_H
#include <iostream>
using namespace std;
class User
{
private:
    string UserName;
    string Password;
public:
    User();
    User(string username, string password);
    string getUserName() const;  //const 限制 this 指针，代表函数内不能修改类成员变量
    void setUserName(const string &value);
    string getPassword() const;
    void setPassword(const string &value);
};

#endif // USER_H
