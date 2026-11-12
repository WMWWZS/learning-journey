#ifndef BICYCLE_H
#define BICYCLE_H
#include "vehicle.h"
class Bicycle : public Vehicle
{
public:
    Bicycle(string name, double speed) : Vehicle(name, speed){}
    void ShowSpeed() const override;
    ~Bicycle() override = default;  //因为继承了，virtual加不加无所谓，为可读性可以添加
};

#endif // BICYCLE_H
