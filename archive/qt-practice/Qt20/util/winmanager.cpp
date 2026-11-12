#include "winmanager.h"

WinManager* WinManager::instence = nullptr;
WinManager *WinManager::getInstence()
{
    if (instence == nullptr)
    {
        instence = new WinManager;
    }
    return instence;
}

WinManager::WinManager()
{

}
