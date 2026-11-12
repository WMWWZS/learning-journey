#include "widget.h"
#include <QApplication>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <iostream>
using namespace std;
using namespace cv;
void test01()
{
    Mat img = imread("E:\\QT_code\\Qt\\Qt11\\image\\blue_100x100.jpg");
    if(img.empty())
    {
        qDebug() << "图片打开失败" ;
        return;
    }

    Mat img_resized;
    resize(img, img_resized, Size(800, 600));

    namedWindow("img", WINDOW_NORMAL); // 允许调整窗口
    resizeWindow("img", 800, 600);     // 设置窗口大小

    imshow("img", img_resized); //
    waitKey(0);
}

void test02()
{
    Mat img = imread("E:\\QT_code\\Qt\\Qt11\\image\\blue_100x100.jpg");
    if(img.empty())
    {
        qDebug() << "图片打开失败" ;
        return;
    }
    for(int i = 0; i < img.rows; ++i)
    {
        for(int j = 0; j < img.cols; j++)
        {
            Vec3b p = img.at<Vec3b>(i,j);  // 获取i，j位置的像素
            //修改该位置像素的值
            p[0] = 255;  //B
            p[1] = 255;  //G
            p[2] = 255;  //R
            qDebug() << (int)p[0] << (int)p[1] << (int)p[2];//`Vec3b`每个分量是`uchar`，直接 qDebug 打印乱码，要强制转`(int)`输出数字
        }
    }
    imshow("img", img);
    waitKey(1);
}

void test03()
{
    //1.读取前景和背景图
    Mat foreground = imread("../image/pkq.png");   //最好图片的背景是纯色，便于
    Mat background = imread("../image/background.jpeg");
    if(foreground.empty() || background.empty())
    {
        qDebug() << "图片读取失败";
        return;
    }

    // 调整背景图尺寸和前景图保持一致
    resize(background, background, foreground.size(), 0, 0, INTER_CUBIC);
    imshow("src", foreground);

    //2. BGR转HSV色彩空间，为什么进行这一步呢？BGR 转 HSV，是为了更容易 更稳定识别白色背景
    Mat hsvImg;
    cvtColor(foreground, hsvImg, COLOR_BGR2HSV);
    imshow("hsv",hsvImg);
    //哪些像素被当作背景剔除，从而影响最终合成图的边缘质量
    Scalar lowerWhite = Scalar(0, 0, 200);   // 0~180 0~50 200~255
    Scalar upperWhite = Scalar(180, 50, 255);  // 色相 饱和度 亮度

    Mat mask;
    //inRange根据阈值生成掩码图  输出掩码mask
    inRange(hsvImg, lowerWhite, upperWhite, mask);
    imshow("mask",mask);
    //创建全0矩阵（黑色画布）
    // 参数1：画布尺寸；参数2：图像类型，和前景图保持一致
    Mat img = Mat::zeros(foreground.size(), foreground.type());
    for(int i = 0; i < foreground.rows; i++)
    {
        for(int j = 0; j < foreground.cols; j++)
        {
            uchar maskPixel = mask.at<uchar>(i, j);
            img.at<Vec3b>(i,j) = maskPixel == 0 ? foreground.at<Vec3b>(i, j) : background.at<Vec3b>(i, j);  // 0就是黑色皮卡丘
        }
    }
    imshow("finish", img);
}

void test04()
{
    //1. 图片的读取
    Mat src = imread("../image/blue_100x100.jpg");
    if(src.empty())
    {
        qDebug() << "图片打开失败";
        return;
    }
    qDebug() << "宽:" << src.cols << " 高:" << src.rows << "通道:" << src.channels();

    // 2.浅拷贝  修改dst1可以影响src
    Mat dst1 = src;
    // 3.深拷贝 全新内存 互不干扰
    Mat dst2 = src.clone();

    // 4.访问彩色元素 Vec3b(B,G,R)
    int x = 20, y = 20;
    Vec3b pixel = src.at<Vec3b>(x, y);
    qDebug() << "B:" << (int)pixel[0] << " G:" << (int)pixel[1] << " R:" << (int)pixel[2];

    dst2.at<Vec3b>(x,y) = Vec3b(0, 2, 5);
    Vec3b pixel2 = dst2.at<Vec3b>(x, y);
    qDebug() << "B:" << (int)pixel2[0] << " G:" << (int)pixel2[1] << " R:" << (int)pixel2[2];

    Mat black = Mat::zeros(Size(400,300), CV_8UC3); //8位无符号通道3 B:bit8 G:bit8 R:bit8
    imshow("black_Mat" , black);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    test04();
    return QApplication::exec();
}
