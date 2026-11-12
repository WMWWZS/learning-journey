#include <iostream>
#include "character.h"
using namespace std;
Character::Character()
{

}

Character::Character(string name, int HP, int attack, int level)
{
    this->name = name;
    this->HP = HP;
    this->attack = attack;
    this->level = level;
}

Character::~Character()
{
    cout << "该实例已析构" << endl;
}

void Character::printfinfo()
{
    cout << "name:" << this->name << "HP:" << this->HP << "attack:" << this->attack << "level:" << this->level << endl;
}
