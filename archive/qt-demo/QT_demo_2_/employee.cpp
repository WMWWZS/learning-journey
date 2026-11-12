#include "employee.h"
#include <string.h>
Employee::Employee() {}

Employee::~Employee()
{
    delete []name;
}

Employee::Employee(const Employee &other)
{
    name = new char[30];
    strcpy(name,other.name);
}

char* Employee::getEmployeeName()
{
    return name;
}













