#ifndef PHONE_H
#define PHONE_H

class Phone
{
public:
    int color;   //颜色
    int screenSize;   //屏幕大小
    int memerySize;  //内存
    char logo[16];
    char type[32];
public:
    Phone();
    Phone(char Logo[], char Type[]);
    void phoneOn(); //开机

    char* getLogo();
    void setLogo(char Togo[]);

    char* getType();
    void setType(char Type[]);

    void call(char phoneNumbers []);  //通话
};

#endif // PHONE_H
