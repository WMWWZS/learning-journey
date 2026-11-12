#ifndef LION_H
#define LION_H
#include "animal.h"
class Lion : public Animal
{
public:
    void speak() const override;
    Lion();
};

#endif // LION_H
