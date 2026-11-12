#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

class Adder  //自定义加法函数对象
{
public:
    int operator()(int a, int b) const   //仿函数（）
    {
        return a + b;
    }
};   //仿函数是类对象，可以自带变量记忆信息，优势

struct Adder2   //默认权限是public
{
    bool operator()(int a, int b) const
    {
        return a > b;
    }
};












