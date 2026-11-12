#include <iostream>
#include "KeyValue.hpp"
#include "MyArray.hpp"
#include "Stack.hpp"
#include <memory>

using namespace std;

// int sum(int a, int b)
// {
//     return a + b;
// }

// double sum(double a, double b)
// {
//     return a + b;
// }

template <typename T>  //T是类型占位符
T sum(T a, T b)
{
    return a + b;
}

int main()
{
    // //自动推导，优先调用普通函数，全部不匹配，才会实例化函数模板
    // double a = 1.4;
    // double b = 1.3;
    // double c = sum<double>(a,b);
    // cout << c << endl;
    // //test01
    // MyArray<int> intArr(3);
    // intArr[0] = 10;
    // cout << intArr[0] << endl;

    // //test02
    // KeyValue<string , double> kv("身高", 1.75);
    // kv.show();

    // //test03
    // Stack<double> st;
    // st.push(3.14);
    // cout << st.top() << endl;

    //首先，智能指针本身就是一种类模板
        //1.unique_ptr（独占式智能指针）
    //特性：
    // 1.一块堆内存只能被一个 unique_ptr 接管，不允许拷贝
    // 2.只可以移动所有权
    // 3.离开作用域自动释放内存
    //一个块堆内存只允许被一个unique_ptr接管 ，允许被拷贝
    unique_ptr<int> p1(new int(66));
    cout << *p1 << endl;

    // unique_ptr<int>p2 = p1  禁止拷贝数据

    unique_ptr<int> p2 = std::move(p1);  //移动所有权，p1将变成空指针
    cout << *p2 << endl;


        //2.shared_ptr（共享计数智能指针）
    //特性：
    //1.多个指针可以共用同一块堆内存
    //2.自带引用计数器，每多一个指针接管，计数 + 1；指针销毁计数?1
    //3.计数器归 0 的时候，自动释放堆内存
    shared_ptr<int> p3 = make_shared<int>(99);
    shared_ptr<int> p4 = p3;
    cout << p3.use_count() << endl;  //当前这个被管理对象（以及对应的控制块）有多少个 shared_ptr 实例在共享拥有/管理它，
    //当前存活的、拥有该对象的 shared_ptr 数量

    p4.reset(); //放弃管理权限
    cout << p3.use_count() << endl;

        //3.weak_ptr（弱引用指针）
    //特性：
    //1.不会增加 shared_ptr 的引用计数
    //2.不能直接解引用，需要先 lock() 转换成 shared_ptr
    //3.用来打破两个 shared_ptr 互相指向对方，造成内存永远无法释放的循环问题


        //类模板继承
    //1.普通类继承类模板



    //2.类模板继承类模板
    //3.类模板继承普通类
    return 0;
}
