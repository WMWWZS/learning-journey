#include <iostream>
#include "logintask.h"
#include "myfstream.h"
#include "regtask.h"
#include "usermodel.h"
using namespace std;

void test01()
{
    string UserName;
    string Password;
    UserModel* task = NULL;
    int choose = 0;
    while(true)
    {
        cout << "1.µÇÂ¼" << endl;
        cout << "2.×¢²á" << endl;
        cout << "ÇëÑ¡Ôñ" << endl;
        cin >> choose;
        cout << "ÇëÊäÈëÓÃ»§Ãû£º" << endl;
        cin >> UserName;
        cout << "ÇëÊäÈëÃÜÂë£º" << endl;
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
    test01();
    return 0;
}
