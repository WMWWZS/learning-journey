#include "widget.h"
#include "facecheck.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FaceCheck w;
    w.show();
    return QApplication::exec();
}
