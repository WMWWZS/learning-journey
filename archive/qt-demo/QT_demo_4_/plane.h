#ifndef PLANE_H
#define PLANE_H
#include "vehicle.h"
class Plane : public Vehicle
{
public:
    Plane(string name, double speed) : Vehicle(name, speed){}
    void ShowSpeed() const override;  //确保派生类中的函数确实覆盖了基类的虚函数
    ~Plane() override = default;
};

#endif // PLANE_H
