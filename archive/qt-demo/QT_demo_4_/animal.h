#ifndef ANIMAL_H
#define ANIMAL_H
class Animal
{
public:
    virtual void speak() const = 0;
    Animal();
    virtual ~Animal();
};

#endif // ANIMAL_H
