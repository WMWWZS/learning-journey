#ifndef MAGE_H
#define MAGE_H
#include "character.h"
class Mage : public Character
{
private:
    int MP; //魔法值 （特有）
public:
    Mage();
    void fireball();
};

#endif // MAGE_H
