#include "facechecktask.h"
using namespace cv;

FaceCheckTask::FaceCheckTask(QObject *parent) : QObject{parent}
{}

void FaceCheckTask::start()
{
    // 1.加载模型 注意：pro文件中是否添加 LIBS += -lopencv_dnn4100        # OpenCV访问Onnx模型的接口模块
    const QString modelPath = QDir(QCoreApplication::applicationDirPath())
    .filePath("weights/yolov8n-face.onnx");

    m_detector = new FaceDetector; //忘记new了
    if (!m_detector->load(modelPath.toStdString()))
    {
        emit sig_error(QString("人脸模型加载失败：%1").arg(modelPath));
        // 模型失败不影响预览，继续往下走
    }
    // 2.打开摄像头
    if (!m_cap.isOpened())
    {
        m_cap.open(0);   //打开摄像头兜底
    }

    if (!m_cap.isOpened())
    {
        emit sig_error("摄像头打开失败");
        return;
    }
    // 3.设置帧规格
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    // 4.用计时器不断触发读取帧数
    m_timer = new QTimer(this);
    m_timer->setInterval(30);   // 约 33fps读帧
    connect(m_timer, &QTimer::timeout, this, &FaceCheckTask::onTimeout);
    m_timer->start();
}

void FaceCheckTask::stop()
{
    if(m_timer)
    {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if (m_cap.isOpened())
    {
        m_cap.release();
    }
}

void FaceCheckTask::setFaceEnable(bool enable)
{
    if(m_faceEnable) //真 关闭 假 开启
    {
        m_faceEnable = false;
    }
    else
    {
        m_faceEnable = true;
    }
}

void FaceCheckTask::onTimeout()
{
    if (!m_cap.isOpened())
    {
        return;
    }

    cv::Mat frame;
    if (!m_cap.read(frame) || frame.empty())
    {
        qDebug() << "frame读取失败";
        return;
    }

    if (m_faceEnable)
    {
        // 每m_detectInterval 帧做一次推理，中间帧沿用上次的框
        if (m_frameCounter % m_detectInterval == 0)  //等叠加到m_detectInterval = 2 时再进行帧推理
        {
            m_lastFaces = m_detector->detect(frame, &m_lastLandmarks);
        }
        ++m_frameCounter;
        FaceDetector::draw(frame, m_lastFaces, m_lastLandmarks);
    }
    emit sig_frameReady(matToQImage(frame));
}

QImage FaceCheckTask::matToQImage(const cv::Mat &mat)
{
    if (mat.empty())
    {
        return QImage();
    }
    if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        // 必须copy()否则QImage只是引用rgb，函数返回后数据就没了
        return QImage(rgb.data, rgb.cols, rgb.rows,
                      static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
    }
    return QImage();
}





