#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;



class fruit
{
public:
    string name;
    double price;
    fruit(const string& s, double p) : name(s) , price(p) {}
    void show()
    {
        cout << "名称：" << name << " " << "价格：" << price << "元" << endl;
    }
};










