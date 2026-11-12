#include "bgreplacewidget.h"
#include <QApplication>
void test()
{
    qDebug() << "OpenCV版本:" << cv::getVersionString();
    qDebug() << "主版本:" << cv::getVersionMajor();
    qDebug() << "次版本:" << cv::getVersionMinor();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BgReplaceWidget test;
    test.show();
    return QApplication::exec();
}