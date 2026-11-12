#include "loginwidget.h"
#include <QApplication>
#include "mainwidget.h"
#include "regwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // loginWidget L;
    // L.show();

    MainWidget M;
    M.show();

    // RegWidget R;
    // R.show();

    return QApplication::exec();
}
