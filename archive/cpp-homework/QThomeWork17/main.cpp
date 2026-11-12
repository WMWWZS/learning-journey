#include "VideoPlay.h"
#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VideoPlay vp;
    vp.show();
    return QApplication::exec();
}
