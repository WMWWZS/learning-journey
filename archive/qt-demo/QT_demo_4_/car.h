#ifndef CAR_H
#define CAR_H
#include "vehicle.h"
class Car : public Vehicle
{
public:
    Car(string name, double speed) : Vehicle(name, speed){}
    void ShowSpeed() const override;
    ~Car() override = default;
};

#endif // CAR_H
