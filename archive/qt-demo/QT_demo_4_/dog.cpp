#include "dog.h"
#include <iostream>
using namespace std;
void Dog::speak() const   //override不允许，类成员函数的声明处使用，不能在类外定义处使用
{
    cout << "汪汪汪" << endl;
}

Dog::Dog() {}
