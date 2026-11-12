#ifndef CAT_H
#define CAT_H
#include "animal.h"
class Cat : public Animal
{
public:
    void speak() const override;
    Cat();
};

#endif // CAT_H
