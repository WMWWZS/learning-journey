#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <string>
#include <vector>
/**
 * @brief 基于 YOLOv8n-face (ONNX) 的人脸检测器
 *
 * 用法：
 *   FaceDetector det;
 *   if (!det.load("weights/yolov8n-face.onnx")) { ... }
 *   std::vector<cv::Rect> faces = det.detect(frame, &landmarks);
 *   FaceDetector::draw(frame, faces, landmarks);
 *
 * 说明：
 *   - 这个类只做"检测 + 画图"，可以单独用 main() 测试。
 *   - 模型输入固定 640x640，等比缩放 + 补边，检测框再映射回原图坐标。
 *   - 输出格式：[1, num_class + 4*reg_max + 15, H, W]
 *       num_class=1, reg_max=16 => 1 + 64 + 15 = 80 个通道
 *       前 64 个是 DFL 的 4 条边分布，第 65 个是类别置信度，后 15 个是 5 个关键点 (x,y,conf)
 */
class FaceDetector
{
public:
    FaceDetector() = default;
    /// 加载 onnx 模型 成功返回 true
    bool load(const std::string &modelPath);
    bool isLoaded() const { return !m_net.empty(); }
    /// 检测人脸，返回原图坐标系下的矩形框；landmarks 非空时同时输出 5 个关键点
    std::vector<cv::Rect> detect(const cv::Mat &frame,
                                 std::vector<std::vector<cv::Point>> *landmarks = nullptr);
    /// 把检测结果画到 frame 上（BGR 三通道）
    static void draw(cv::Mat &frame,
                     const std::vector<cv::Rect> &faces,
                     const std::vector<std::vector<cv::Point>> &landmarks = {});
    void setConfThreshold(float v) { m_confThreshold = v; }
    void setNmsThreshold(float v) { m_nmsThreshold = v; }
private:
    cv::Mat resizeImage(const cv::Mat &src, int *newh, int *neww, int *padh, int *padw) const;
    void generateProposal(const cv::Mat &out,
                          std::vector<cv::Rect> &boxes,
                          std::vector<float> &confidences,
                          std::vector<std::vector<cv::Point>> &landmarks,
                          int imgh, int imgw,
                          float ratioh, float ratiow,
                          int padh, int padw) const;
    cv::dnn::Net m_net;
    const int   m_inpWidth  = 640;
    const int   m_inpHeight  = 640;
    const int  m_numClass     = 1;   // 只有人脸face 一类
    const int   m_regMax       = 16;  // DFL 的 bin 数
    float   m_confThreshold = 0.45f;
    float       m_nmsThreshold  = 0.50f;
};
#endif // FACEDETECTOR_H