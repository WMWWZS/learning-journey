#ifndef SON2_H
#define SON2_H
#include "father.h"
class Son2 : protected Father   //父类 public/protected 全部降级为子类 protected
{
public:
    Son2();
};

#endif // SON2_H
