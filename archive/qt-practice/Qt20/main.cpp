#include <QApplication>
#include "view/regwidget.h"
#include "util/winmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WinManager::getInstence()->mainWidget.show();


    return QApplication::exec();
}
