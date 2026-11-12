#ifndef ARCHER_H
#define ARCHER_H
#include "character.h"
// class Archer
// {
// private:
//     int agility; //敏捷 （特有）
// public:
//     void archery();
// };  //这是错误的，应该是在基类基础上进行继承

class Archer : public Character    //控制父类成员在子类里的访问权限
{
private:
    int agility;  //敏捷
public:
    Archer();
    void archery();
};

#endif // ARCHER_H
