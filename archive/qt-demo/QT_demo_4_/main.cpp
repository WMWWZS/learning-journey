#include <iostream>
#include "basetask.h"
#include "cat.h"
#include "dog.h"
#include "lion.h"
#include "vehicle.h"
#include "plane.h"
#include "car.h"
#include "bicycle.h"
#include "user.h"
#include "logintask.h"
#include "regtask.h"
using namespace std;
void test01()
{
    User user("admin", "123456");
    BaseTask* task = new LoginTask(&user, sizeof(user));
    task->service();
    delete task;
}
void test02()
{
    User user("admin", "123456");
    BaseTask* task = new RegTask(&user, sizeof(user));
    task->service();
    delete task;
}
void test03()
{
    string UserName;
    string Password;
    BaseTask* task = NULL;
    int choose = 0;
    while(true)
    {
        cout << "1.登录" << endl;
        cout << "2.注册" << endl;
        cout << "请选择" << endl;
        cin >> choose;
        cout << "请输入用户名：" << endl;
        cin >> UserName;
        cout << "请输入密码：" << endl;
        cin >> Password;
        User u(UserName, Password);
        if(choose == 1)
        {
            task = new LoginTask(&u, sizeof(User));
        }
        else if(choose == 2)
        {
            task = new RegTask(&u, sizeof(User));
        }
        task->service();
        delete task;
        task = NULL;
    }
}

int main()
{
    // Dog dog;
    // Lion lion;
    // Cat cat;
    // cat.speak();
    // dog.speak();
    // lion.speak();
    // Plane p("飞机", 445.0);
    // Car c("汽车", 46.9);
    // Bicycle b("自行车", 25.3);
    // p.ShowSpeed();
    // c.ShowSpeed();
    // b.ShowSpeed();
    test01();
    test02();
    test03();
    return 0;
}
