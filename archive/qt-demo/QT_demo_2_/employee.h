#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee
{
public:
    char* name;
    Employee();
    Employee(const Employee& other); //系统默认
    ~Employee();
    char* getEmployeeName();   //构造函数不能有返回值
};

#endif // EMPLOYEE_H
