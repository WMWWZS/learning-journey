#ifndef WARRIOR_H
#define WARRIOR_H
#include "character.h"
class Warrior : public Character
{
private:
    int defense; //∑¿”˘ £®Ãÿ”–£©
public:
    Warrior();
    Warrior(string name, int HP, int attack, int level, int defense);
    void printfinfo();
    ~Warrior();
    void charge();
};

#endif // WARRIOR_H
