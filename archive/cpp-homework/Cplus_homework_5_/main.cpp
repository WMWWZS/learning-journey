#include <iostream>
#include "linkedlist.hpp"
#include "CircularList.hpp"
using namespace std;

int main()
{
    // Linkedlist<int> list;
    // list.pushFront(10);   //int &a = 10; 错误，普通引用不能绑定临时值，所以函数加const
    // list.pushFront(20);
    // list.pushFront(30);
    // list.pushFront(40);
    // int a= list.get(3);
    // cout << a << endl;
    // cout << "size: " << list.getSize() << endl;
    // list.debugPrint();
    int temp1 = 3;
    int temp2 = 2;
    CircularList<int> Clist;
    Clist.pushFront(1);
    Clist.pushFront(2);
    Clist.pushFront(3);
    Clist.pushFront(4);
    Clist.pushBack(47);
    Clist.debugPrint();

    Clist.popBack(temp1);
    Clist.debugPrint();

    Clist.popFront(temp2);
    Clist.debugPrint();
    return 0;
}
