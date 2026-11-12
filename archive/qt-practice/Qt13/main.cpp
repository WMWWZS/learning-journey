#include "widget.h"

#include <QApplication>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
void test02()
{
    //1.白色画布
    Mat canvas(Size(800, 600), CV_8UC3, Scalar(255,255,255));

    //2.画直线 p1起点 p2终点 Scalar 0 0 255 ，线条粗细 线条材质
    line(canvas, Point(100,100), Point(480, 380), Scalar(0,0,255), 2,LINE_AA);

    rectangle(canvas, Point(200, 150), Point(50, 50), Scalar(0, 255, 0), 2);
    //3.矩形：对角点；thickness=-1代表填充
    //从250 50 坐标开始 画一个宽150 高100的矩形
    rectangle(canvas, Rect(250,50,150,100), Scalar(255, 0, 0), 0);

    //4.圆形：圆心，半径
    circle(canvas, Point(250, 250), 80, Scalar(0,255,255),3,LINE_AA);

    //5.绘制文字putText(图像，文字，起始坐标，字体，缩放大小，颜色，粗细，线形）
    putText(canvas, "WuShiShuai", Point(20,580), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0,0,0), 2 ,LINE_AA);
    // imshow("组合", canvas);

    Mat src = imread("../image/background.jpg");
    if(src.empty())
    {
        qDebug() << "图片打开失败";
        exit(0);
    }

    Mat src2 = src.clone();
    resize(src, src2, Size(src.cols / 2, src.rows / 2));

    putText(src2, "Test", Point(src2.cols - 525,src2.rows - 20), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0,0,255), 2 ,LINE_AA);
    namedWindow("显示结果", WINDOW_NORMAL);
    imshow("显示结果", canvas);
    imshow("水印测试", src2);
    waitKey(0);
    exit(0);
}

void test01()
{
    VideoCapture cap("../video/ovals_04.mp4");
    if(!cap.isOpened())
    {
        qDebug() << "视频打开失败，检查路径/解码器";
        return;
    }

    // 视频的一些属性get
    double fps = cap.get(CAP_PROP_FPS);
    int width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    int total_frame = static_cast<long>(cap.get(CAP_PROP_FRAME_COUNT));//显式转化

    qDebug() << "fps:" << fps << "分辨率:" << width << "x" << height
             << "总帧数:" << total_frame;

    //视频保存
    int fourcc = VideoWriter::fourcc('M', 'J', 'P', 'G');
    VideoWriter writer("out_video.avi", fourcc, fps, Size(width, height));
    if(!writer.isOpened())
    {
        qDebug() << "初始化失败";
        cap.release();
        return;
    }

    namedWindow("video playing", WINDOW_NORMAL);
    resizeWindow("video playing", 960, 540); // 限制为 960x540 播放

    Mat frame;
    while(cap.read(frame))
    {

        putText(frame, "DEMO_VIDEO", Point(frame.cols*0.85, frame.rows-20),
                FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0,0,255), 2);

        writer.write(frame);
        imshow("video playing", frame);

        if(waitKey(1) == 'q')
            break;
    }

    cap.release();
    writer.release();
    destroyAllWindows();
    exit(1);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    test01();
    return QApplication::exec();
}
