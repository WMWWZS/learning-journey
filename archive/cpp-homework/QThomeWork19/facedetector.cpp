#include "facedetector.h"
#include <algorithm>
#include <array>
#include <cmath>
using namespace cv;
static inline float sigmoidf(float x)
{
    return 1.0f / (1.0f + std::exp(-x));
}
bool FaceDetector::load(const std::string &modelPath)
{
    try {
        m_net = dnn::readNet(modelPath);
    } catch (const cv::Exception &e) {
        return false;
    }
    return !m_net.empty();
}
Mat FaceDetector::resizeImage(const Mat &src, int *newh, int *neww, int *padh, int *padw) const
{
    const int srch = src.rows;
    const int srcw = src.cols;
    *newh = m_inpHeight;
    *neww = m_inpWidth;
    *padh = 0;
    *padw = 0;
    Mat dst;
    if (srch != srcw) {
        // 等比缩放 + 补边，避免人脸被拉变形
        const float hw_scale = static_cast<float>(srch) / srcw;
        if (hw_scale > 1.0f) {
            *newh = m_inpHeight;
            *neww = static_cast<int>(m_inpWidth / hw_scale);
            resize(src, dst, Size(*neww, *newh), INTER_AREA);
            *padw = static_cast<int>((m_inpWidth - *neww) * 0.5);
            copyMakeBorder(dst, dst, 0, 0, *padw, m_inpWidth - *neww - *padw,
                           BORDER_CONSTANT, Scalar(0, 0, 0));
        } else {
            *newh = static_cast<int>(m_inpHeight * hw_scale);
            *neww = m_inpWidth;
            resize(src, dst, Size(*neww, *newh), INTER_AREA);
            *padh = static_cast<int>((m_inpHeight - *newh) * 0.5);
            copyMakeBorder(dst, dst, *padh, m_inpHeight - *newh - *padh, 0, 0,
                           BORDER_CONSTANT, Scalar(0, 0, 0));
        }
    } else {
        resize(src, dst, Size(*neww, *newh), INTER_AREA);
    }
    return dst;
}
void FaceDetector::generateProposal(const Mat &out,
                                    std::vector<Rect> &boxes,
                                    std::vector<float> &confidences,
                                    std::vector<std::vector<Point>> &landmarks,
                                    int imgh, int imgw,
                                    float ratioh, float ratiow,
                                    int padh, int padw) const
{
    if (out.dims != 4) {
        return;
    }
    const int feat_h = out.size[2];
    const int feat_w = out.size[3];
    const int stride = static_cast<int>(std::ceil(static_cast<float>(m_inpHeight) / feat_h));
    const int area = feat_h * feat_w;
    const float *ptr      = reinterpret_cast<const float *>(out.data);
    const float *ptr_cls  = ptr + area * m_regMax * 4;
    const float *ptr_kp   = ptr + area * (m_regMax * 4 + m_numClass);
    for (int i = 0; i < feat_h; ++i) {
        for (int j = 0; j < feat_w; ++j) {
            const int index = i * feat_w + j;
            float maxConf = -10000.0f;
            for (int k = 0; k < m_numClass; ++k) {
                maxConf = std::max(maxConf, ptr_cls[k * area + index]);
            }
            const float boxProb = sigmoidf(maxConf);
            if (boxProb <= m_confThreshold) {
                continue;
            }
            // DFL：把 4 条边的分布求期望，得到 l/t/r/b 四个距离
            float pred_ltrb[4];
            for (int k = 0; k < 4; ++k) {
                std::array<float, 16> dfl{};
                for (int n = 0; n < m_regMax; ++n) {
                    dfl[n] = ptr[(k * m_regMax + n) * area + index];
                }
                float maxv = *std::max_element(dfl.begin(), dfl.end());
                float sum = 0.0f;
                std::array<float, 16> prob{};
                for (int n = 0; n < m_regMax; ++n) {
                    prob[n] = std::exp(dfl[n] - maxv);
                    sum += prob[n];
                }
                float dis = 0.0f;
                for (int n = 0; n < m_regMax; ++n) {
                    dis += n * (prob[n] / sum);
                }
                pred_ltrb[k] = dis * stride;
            }
            const float cx = (j + 0.5f) * stride;
            const float cy = (i + 0.5f) * stride;
            const float xmin = std::max((cx - pred_ltrb[0] - padw) * ratiow, 0.0f);
            const float ymin = std::max((cy - pred_ltrb[1] - padh) * ratioh, 0.0f);
            const float xmax = std::min((cx + pred_ltrb[2] - padw) * ratiow, static_cast<float>(imgw - 1));
            const float ymax = std::min((cy + pred_ltrb[3] - padh) * ratioh, static_cast<float>(imgh - 1));
            if (xmax <= xmin || ymax <= ymin) {
                continue;
            }
            boxes.emplace_back(Rect(static_cast<int>(xmin), static_cast<int>(ymin),
                                    static_cast<int>(xmax - xmin), static_cast<int>(ymax - ymin)));
            confidences.push_back(boxProb);
            std::vector<Point> kpts(5);
            for (int k = 0; k < 5; ++k) {
                const float x = ((ptr_kp[(k * 3) * area + index] * 2 + j) * stride - padw) * ratiow;
                const float y = ((ptr_kp[(k * 3 + 1) * area + index] * 2 + i) * stride - padh) * ratioh;
                kpts[k] = Point(static_cast<int>(x), static_cast<int>(y));
            }
            landmarks.push_back(kpts);
        }
    }
}
std::vector<Rect> FaceDetector::detect(const Mat &frame, std::vector<std::vector<Point>> *landmarks)
{
    std::vector<Rect> faces;
    if (m_net.empty() || frame.empty()) {
        return faces;
    }
    if (landmarks)
    {
        landmarks->clear();   //每次检测前清空，否则五个点一直沿用第一帧
    }
    int newh = 0, neww = 0, padh = 0, padw = 0;
    const Mat dst = resizeImage(frame, &newh, &neww, &padh, &padw);
    Mat blob;
    dnn::blobFromImage(dst, blob, 1.0 / 255.0,
                       Size(m_inpWidth, m_inpHeight),
                       Scalar(0, 0, 0), true, false); // BGR->RGB, 不裁剪
    m_net.setInput(blob);
    std::vector<Mat> outs;
    m_net.forward(outs, m_net.getUnconnectedOutLayersNames());
    std::vector<Rect> boxes;
    std::vector<float> confidences;
    std::vector<std::vector<Point>> kptsAll;
    const float ratioh = static_cast<float>(frame.rows) / newh;
    const float ratiow = static_cast<float>(frame.cols) / neww;
    for (const Mat &out : outs) {
        generateProposal(out, boxes, confidences, kptsAll,
                         frame.rows, frame.cols, ratioh, ratiow, padh, padw);
    }
    std::vector<int> indices;
    dnn::NMSBoxes(boxes, confidences, m_confThreshold, m_nmsThreshold, indices);
    faces.reserve(indices.size());
    for (int idx : indices) {
        faces.push_back(boxes[idx]);
        if (landmarks) {
            landmarks->push_back(kptsAll[idx]);
        }
    }
    return faces;
}
void FaceDetector::draw(Mat &frame,
                        const std::vector<Rect> &faces,
                        const std::vector<std::vector<Point>> &landmarks)
{
    for (size_t i = 0; i < faces.size(); ++i) {
        const Rect &f = faces[i];
        rectangle(frame, f, Scalar(0, 255, 0), 2);
        if (i < landmarks.size()) {
            for (const Point &p : landmarks[i]) {
                circle(frame, p, 2, Scalar(0, 0, 255), -1);
            }
        }
    }
    const std::string label = "faces: " + std::to_string(faces.size());
    putText(frame, label, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 255, 0), 2);
}