#include "widget.h"
#include "camerawidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CameraWidget C;
    C.show();
    return QApplication::exec();
}
