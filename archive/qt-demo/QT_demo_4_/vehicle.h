#ifndef VEHICLE_H
#define VEHICLE_H
#include <iostream>
using namespace std;
class Vehicle
{
protected:
    string name;
    double speed;
public:
    Vehicle(string n, double s) : name(n) , speed(s) {}  //接收n,s 给 name和speed初始化
    virtual void ShowSpeed() const = 0;  //override只能用在派生类中覆盖基类虚函数的成员函数上，不能用于普通函数。
    virtual ~Vehicle(){}  //一旦基类将析构函数声明为virtual，所有派生类的析构函数都会自动成为虚函数
};

#endif // VEHICLE_H
