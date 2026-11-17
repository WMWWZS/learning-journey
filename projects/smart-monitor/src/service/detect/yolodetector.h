#ifndef YOLODETECTOR_H
#define YOLODETECTOR_H

// ============================================================================
// YoloDetector：YOLOv8 目标检测（OpenCV DNN，CPU）
//
// 作用：
//   把一帧 cv::Mat(BGR) 丢进去，返回“框 + 置信度 + 类别”。
//   不依赖 Qt，可单独测试；线程安全性：一个实例只在同一个线程里用。
//
// 模型：
//   bin/weights/yolov8n.onnx（YOLOv8n，COCO 80 类，person = 0）
//   输出 3 个张量，形状 (1,144,80,80)/(1,144,40,40)/(1,144,20,20)
//   —— 前 64 通道是 DFL 框分布，后 80 通道是类别 logits
//
// 参考：
//   https://github.com/YYH-GPU/yolov11-yolov8-dnn-Classify-Detect-Obb-Pose-Seg
// ============================================================================

#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

struct Detection
{
    cv::Rect box;      //原图坐标下的框
    float score;    //置信度 0~1
    int classId;  //类别下标（0 = person）
};

class YoloDetector
{
public:
    YoloDetector() = default;

    // 加载 onnx 模型；成功返回 true
    bool load(const std::string &onnxPath,
              float confThreshold = 0.25f,   //置信度阈值，小于这个值直接丢弃，不是检测目标
              float nmsThreshold  = 0.45f);   //叠比较多才删 调大，更容易删掉重叠框 调小，容易保留多个重复框

    bool isLoaded() const { return !m_net.empty(); }

    //对一帧BGR图做检测（内部自动 letterbox 到 640x640）
    std::vector<Detection> detect(const cv::Mat &bgr);

    //画绿色框+类别/置信度文字（返回绘制后的图）
    static void drawGreenBoxes(cv::Mat &bgr, const std::vector<Detection> &dets);

    //只统计人的数量
    static int countPersons(const std::vector<Detection> &dets);

private:
    cv::dnn::Net m_net;
    int m_inputSize = 640;   // 模型输入尺寸
    float m_conf = 0.25f;
    float m_nms = 0.45f;
    bool m_loaded = false;

    static constexpr int kNumBins = 16;  // DFL 的 16 个 bin
    static constexpr int kNumClasses = 80;  // COCO 类别数
    static constexpr int kBoxCh = 64;  // 4 * 16

    static float sigmoid(float x);  //constexpr编译时期求值
    static void  softmax(float *x, int n);
};

#endif // YOLODETECTOR_H
