#ifndef BASE_HPP
#define BASE_HPP
#include <iostream>
using namespace std;

//1.基类模板
template <typename T>
class base
{
protected:
    T data;
public:
    base(const T& v) : data(v) {};
    T get() const
    {
        return data;
    }

    void show() const
    {
        cout << "Base value: " << data << endl;
    }
    virtual void greet() const
    {
        cout << "Hello from Base" << endl;
    }
};


//2.普通类继承模板
class base2 : public base<int>
{
    base2() : base<int>(0) {}; //先让父类构造完
    explicit base2(int v) : base<int>(v) {};  //禁止隐式转化
    void set(int v)
    {
        data = v;
    }
    void doubleValue()
    {
        data *= 2;
    }
    void greet() const override
    {
        cout << "Hello from Base2" << endl;
    }
};

//3.类模板继承类模板
template <typename T>  //忘记了
class base3 : public base<T>
{
    base3(const T& v) : base<T>(v){};
    void print() const
    {
         cout << "Derived value: " << this->data << endl;
    }

    void greet() const override
    {
        cout << "Hello from Base3" << endl;
    }
};
//初始化列表的先后顺序无效，只看成员在类内从上往下的排布
//4.派生类模板带额外参数
template <typename T, typename ExtraType>
class base4 : public base<T>   //别忘了这个需要指定<>
{

};
#endif // BASE_HPP
