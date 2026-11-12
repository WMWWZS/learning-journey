#ifndef USER_H
#define USER_H
#include <iostream>
using namespace std;
class User
{
private:
    int userId;
    string UserName;
    string UserPassword;
    bool state;
public:
    static int UserNum;
    User();
    User(string UserName, string UserPassword, bool state = false);

    const int getUserId() const;
    const string getUserName() const;
    const string getUserPassword() const;
    const bool getUserState() const;

    void setUserName(string UserName);
    void setUserPassword(string UserName);
    void setUserState(bool state);
};

#endif // USER_H