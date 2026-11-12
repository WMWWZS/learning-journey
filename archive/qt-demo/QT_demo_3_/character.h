#ifndef CHARACTER_H
#define CHARACTER_H
#include <iostream>
using namespace std;
class Character
{
protected:    //仅本类和子类可访问
    string name;
    int HP;
    int level;
    int attack;
public:
    Character();
    Character(string name, int HP, int attack, int level);
    ~Character();
    void printfinfo();
};

#endif // CHARACTER_H
