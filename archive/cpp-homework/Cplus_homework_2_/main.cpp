#include <iostream>
#include "user.h"
#include "usermodel.h"
using namespace std;

int main()
{
    cout << "µÇÂ¼¡ª¡ª¡ª¡ª×¢²á_test" <<endl;
    UserModel UM;
    string name;
    string password;

    cout << "×¢²á²âÊÔ" << endl;
    cout << "ÇëÊäÈëname£º" << endl;
    cin >> name;

    cout << "ÇëÊäÈëpassword£º" << endl;
    cin >> password;
    UM.regiserUser(name, password);

    cout << "µÇÂ¼²âÊÔ" << endl;
    cout << "ÇëÊäÈëname£º" << endl;
    cin >> name;

    cout << "ÇëÊäÈëpassword£º" << endl;
    cin >> password;
    UM.loginUser(name, password);


    return 0;
}