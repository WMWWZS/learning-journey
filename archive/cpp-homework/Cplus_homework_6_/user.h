#ifndef USER_H
#define USER_H
#include <iostream>
using namespace std;



class user
{
private:
    int userId;
    string userName;
    string passWord;
    int state;
    static int seq;
public:
    user(string name, string pwd, int state = 1);
    int getUserId() const;
    string getUserName() const;
    void setUserName(const string &newUserName);
    string getPassWord() const;
    void setPassWord(const string &newPassWord);
    int getState() const;
    void setState(int newState);
    static int getSeq();
};

#endif // USER_H
