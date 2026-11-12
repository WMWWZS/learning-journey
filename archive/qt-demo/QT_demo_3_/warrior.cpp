#include "warrior.h"
Warrior::Warrior()
{

}

Warrior::Warrior(string name, int HP, int attack, int level, int defense) : Character(name, HP, attack , level)  //这里我Character(name),Character(HP)...
{
    this->defense = defense;
}

void Warrior::printfinfo()
{
    cout << "骑士冲锋" << "防御力：" << defense << endl;
}

Warrior::~Warrior()
{

}

void Warrior::charge()
{
    cout << "Warrior战士销毁" << endl;
}
