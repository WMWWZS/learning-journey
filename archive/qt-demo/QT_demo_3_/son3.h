#ifndef SON3_H
#define SON3_H
#include "father.h"
class Son3 : private Father   //父类 public/protected 全部降级为子类 private
{
public:
    Son3();
};

#endif // SON3_H
