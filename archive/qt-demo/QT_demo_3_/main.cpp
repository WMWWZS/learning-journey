#include "character.h"
#include "father.h"
#include "son1.h"
#include "son2.h"
#include "son3.h"
int main()
{
    //对象的进行public protected private的权限变化不再赘述
    //向上转型和向下转型            //向上：子到父
    Son1 son1;
    Father* fat = &son1;
    fat->FgetSomethime();  //转完后，只能通过这个指针用父类的东西，子类特有的东西看不见了
    Father* fat2 = new Son1();  //先向上
    Son1* s1 = (Son1*)fat2;   //可以利用dynamic_cast前提是父类必须有虚函数
    cout << "到这了" << endl;
    return 0;
}