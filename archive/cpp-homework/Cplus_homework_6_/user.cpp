#include "user.h"
int user::seq = 1;    //由于忘记static初始化，导致错误

user::user(string name, string pwd, int state)
{
    userName = name;
    passWord = pwd;
    this->state = state;
    userId = seq++; //先赋值，再序列号+1
}

string user::getUserName() const
{
    return userName;
}

void user::setUserName(const string &newUserName)
{
    userName = newUserName;
}

string user::getPassWord() const
{
    return passWord;
}

void user::setPassWord(const string &newPassWord)
{
    passWord = newPassWord;
}

int user::getState() const
{
    return state;
}

void user::setState(int newState)
{
    state = newState;
}

int user::getSeq()
{
    return seq;
}

int user::getUserId() const
{
    return userId;
}
