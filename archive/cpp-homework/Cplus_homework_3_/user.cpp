#include "user.h"
int User::UserNum = 0;   //原先是我这等于10，直接导致登录池满


User::User() {}

User::User(string UserName, string UserPassword, bool state)
{
    // this->userId = ++UserNum;
    // strcpy(this->UserName, UserName);
    // strcpy(this->UserPassword, UserPassword);
    // this->state = state;
    this->userId = ++UserNum;
    this->UserName = UserName;
    this->UserPassword = UserPassword;
    this->state = state;
}

const int User::getUserId() const
{
    return this->userId;
}

const string User::getUserName() const
{
    return this->UserName;
}

const string User::getUserPassword() const
{
    return this->UserPassword;
}

const bool User::getUserState() const
{
    return this->state;
}

void User::setUserName(string UserName)
{
    this->UserName = UserName;
}

void User::setUserPassword(string UserPassword)
{
    this->UserPassword = UserPassword;
}

void User::setUserState(bool state)
{
    this->state = state;
}