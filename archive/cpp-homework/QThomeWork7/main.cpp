#include "loginwidget.h"
#include <QApplication>
#include "regwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginWidget L;
    L.show();

    // RegWidget R;
    // R.show();
    return QApplication::exec();
}
