#ifndef WINMANAGER_H
#define WINMANAGER_H
#include "../view/loginwidget.h"
#include "../view/mainwidget.h"
#include "../view/videowidget.h"

class WinManager
{
public:
    WinManager(const WinManager&) = delete ;
    WinManager& operator=(const WinManager&) = delete ;
    static WinManager* getInstence();
    MainWidget mainWidget;
    loginWidget loginWidet;
private:
    static WinManager* instence;
    WinManager();
};

#endif // WINMANAGER_H
