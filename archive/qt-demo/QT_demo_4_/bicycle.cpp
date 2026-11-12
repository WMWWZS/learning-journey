#include "bicycle.h"

void Bicycle::ShowSpeed() const
{
    cout << "这是：" << this->name << "  "<< "速度是：" << this->speed  << "km/h" << endl;
}