#include "bgreplacewidget.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QHBoxLayout>

BgReplaceWidget::BgReplaceWidget(QWidget *parent) : QWidget{parent}
{
    this->resize(800, 600);
    //外层
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(12);  //控件之间的间距
    vLayout->setContentsMargins(20,20,20,20);  //边距：左上右下

    //顶端水平布局
    QHBoxLayout* hBtnLayout = new QHBoxLayout(this);
    m_btnloadfront = new QPushButton("1.加载前景图（需要纯白背景）");
    m_btnloadback = new QPushButton("2.加载新背景图");
    m_btnRun = new QPushButton("3.执行背景替换");

    hBtnLayout->addWidget(m_btnloadfront);
    hBtnLayout->addWidget(m_btnloadback);
    hBtnLayout->addWidget(m_btnRun);

    //把按钮水平布局加入总垂直布局
    vLayout->addLayout(hBtnLayout);


    m_labelShow = new QLabel;
    m_labelShow->setStyleSheet("border:1px solid #888888; background:#eeeeee;");
    m_labelShow->setMinimumSize(400,300); //最小尺寸，防止窗口缩太小控件消失
    vLayout->addWidget(m_labelShow, 1);

    //信号槽绑定
    connect(m_btnloadfront, &QPushButton::clicked, this, &BgReplaceWidget::slotLoadFrontImg);
    connect(m_btnloadback, &QPushButton::clicked, this, &BgReplaceWidget::slotLoadBackImg);
    connect(m_btnRun, &QPushButton::clicked, this, &BgReplaceWidget::slotReplaceBg);
}

QImage BgReplaceWidget::cvMat2QImage(const cv::Mat &mat)
{
    if(mat.type() == CV_8UC3) //Mat默认的配置
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        QImage imge(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        return imge.copy();   //标记bug 之前是return imge的临时变量 造成图片无法显示 ！！！！！！！！！！！！！！！！！！！！！！！！！！！
    }
    return QImage();
}
void BgReplaceWidget::slotLoadFrontImg()  //加载前景
{
    QString filePath = QFileDialog::getOpenFileName(this,"选择前景图片",".","图片文件 (*.jpg *.png *.bmp)"); //呼出文件选择器 第三个参数的意思是打开当前exe所运行的目录
    if(filePath.isEmpty())
    {        //(*.jpg *.png *.bmp) 括号中写不需要过滤掉的文件格式
        qDebug() << "路径错误" ;
        return;
    }
    m_frontMat = cv::imread(filePath.toStdString());

    if(m_frontMat.empty())
    {
        qDebug() << "图片错误" ;
        return;
    }

    QImage img = cvMat2QImage(m_frontMat);
    m_labelShow->setPixmap(QPixmap::fromImage(img).scaled(m_labelShow->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void BgReplaceWidget::slotLoadBackImg()  //加载背景
{
    QString filePath = QFileDialog::getOpenFileName(this,"选择背景图片",".","图片文件 (*.jpg *.png *.bmp)"); //呼出文件选择器 第三个参数的意思是打开当前exe所运行的目录
    if(filePath.isEmpty())                                     //(*.jpg *.png *.bmp) 括号中写不需要过滤掉的文件格式
    {
        qDebug() << "路径错误" ;
        return;
    }


    m_backMat = cv::imread(filePath.toStdString());

    if(m_backMat.empty())
    {
        qDebug() << "图片错误" ;
        return;
    }

    QImage img = cvMat2QImage(m_backMat);
    m_labelShow->setPixmap(QPixmap::fromImage(img).scaled(m_labelShow->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void BgReplaceWidget::slotReplaceBg()     //替换
{
    if(m_backMat.empty() || m_frontMat.empty())
    {
        qDebug() << "图片读取失败";
        return;
    }

    // 1.调整背景图尺寸和前景图保持一致
    //  原错误 cv::resize(m_backMat, m_frontMat, m_frontMat.size(), 0, 0, cv::INTER_CUBIC);
    cv::Mat t_Mat;
    cv::resize(m_backMat, t_Mat, m_frontMat.size(), 0, 0, cv::INTER_CUBIC);

    //2. BGR转HSV色彩空间，为什么进行这一步呢？BGR 转 HSV，是为了更容易 更稳定识别白色背景
    cv::Mat hsvImg;
    cvtColor(m_frontMat, hsvImg, cv::COLOR_BGR2HSV);


    //哪些像素被当作背景剔除，从而影响最终合成图的边缘质量
    cv::Scalar lowerWhite = cv::Scalar(0, 0, 200);   // 0~180 0~50 200~255
    cv::Scalar upperWhite = cv::Scalar(180, 50, 255);  // 色相 饱和度 亮度


    cv::Mat mask;
    //inRange根据阈值生成掩码图  输出掩码mask
    inRange(hsvImg, lowerWhite, upperWhite, mask);


    //创建全0矩阵（黑色画布）
    // 参数1：画布尺寸；参数2：图像类型，和前景图保持一致
    cv::Mat img = cv::Mat::zeros(m_frontMat.size(), m_frontMat.type());
    for(int i = 0; i < m_frontMat.rows; i++)
    {
        for(int j = 0; j < m_frontMat.cols; j++)
        {
            uchar maskPixel = mask.at<uchar>(i, j);   //掩码通常是单通道灰度图，所以直接用 mask.at<uchar>(i, j) 读取，不用像彩色图那样用 Vec3b
            img.at<cv::Vec3b>(i,j) = (maskPixel == 0) ? m_frontMat.at<cv::Vec3b>(i, j) : m_backMat.at<cv::Vec3b>(i, j);  // 0就是黑色皮卡丘
        }   //img.at<cv::Vec3b>(i, j)，意思是获取坐标 (i, j) 处包含3个 uchar 的向量即B G R三个通道的值
    }

    QImage img2 = cvMat2QImage(img);
    m_labelShow->setPixmap(QPixmap::fromImage(img2).scaled(m_labelShow->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));  //如何做到铺满不变形呢
}
