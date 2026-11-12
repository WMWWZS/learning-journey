#include "user.h"

User::User()
{
    UserName = "";
    Password = "";
}

User::User(string username, string password)
{
    this->UserName = username;
    this->Password = password;
}

string User::getUserName() const
{
    return UserName;
}

void User::setUserName(const string &value)
{
    UserName = value;
}

string User::getPassword() const
{
    return Password;
}

void User::setPassword(const string &value)
{
    Password = value;
}
