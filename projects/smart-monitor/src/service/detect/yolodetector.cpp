#include "yolodetector.h"

#include <algorithm>
#include <cmath>

// ---------------------------------------------------------------------------
// 激活函数
// ---------------------------------------------------------------------------
float YoloDetector::sigmoid(float x)
{
    return 1.0f / (1.0f + std::exp(-x));
}

void YoloDetector::softmax(float *x, int n)
{
    float maxv = x[0];
    for (int i = 1; i < n; ++i) maxv = std::max(maxv, x[i]);

    float sum = 0.0f;
    for (int i = 0; i < n; ++i) { x[i] = std::exp(x[i] - maxv); sum += x[i]; }
    for (int i = 0; i < n; ++i) x[i] /= sum;
}

// ---------------------------------------------------------------------------
// 加载模型
// ---------------------------------------------------------------------------
bool YoloDetector::load(const std::string &onnxPath, float confThreshold, float nmsThreshold)
{
    m_conf = confThreshold;
    m_nms  = nmsThreshold;
    try
    {
        m_net = cv::dnn::readNetFromONNX(onnxPath);
    }
    catch (const cv::Exception &e)
    {
        m_loaded = false;
        return false;
    }
    if (m_net.empty()) { m_loaded = false; return false; }

    m_net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    m_net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    m_loaded = true;
    return true;
}

// ---------------------------------------------------------------------------
// 核心：检测
//
// 流程：letterbox 640x640 → blob → forward(3 个输出) → 逐格解 DFL + 类别
//       → 还原到原图 → NMS
// ---------------------------------------------------------------------------
std::vector<Detection> YoloDetector::detect(const cv::Mat &bgr)
{
    std::vector<Detection> result;
    if (!m_loaded || bgr.empty()) return result;

    // 1) letterbox：等比缩放 + 居中填充，避免直接 resize 拉伸导致框不准
    const int S = m_inputSize;
    const float scale = std::min(float(S) / bgr.cols, float(S) / bgr.rows);
    const int nw = int(bgr.cols * scale);
    const int nh = int(bgr.rows * scale);
    const int ox = (S - nw) / 2;
    const int oy = (S - nh) / 2;

    cv::Mat canvas(S, S, bgr.type(), cv::Scalar(114, 114, 114));
    cv::resize(bgr, canvas(cv::Rect(ox, oy, nw, nh)), cv::Size(nw, nh));

    // 2) 转 blob：1/255、swapRB=true、不裁剪
    cv::Mat blob;
    cv::dnn::blobFromImage(canvas, blob, 1.0 / 255.0, cv::Size(S, S),
                           cv::Scalar(), true, false);
    m_net.setInput(blob);

    std::vector<cv::Mat> outs;
    m_net.forward(outs, m_net.getUnconnectedOutLayersNames());

    std::vector<cv::Rect> boxes;
    std::vector<float>    scores;
    std::vector<int>      classIds;

    // 3) 解析每个尺度的输出：(1, 144, G, G)
    for (const cv::Mat &out : outs)
    {
        if (out.dims != 4 || out.size[1] != kBoxCh + kNumClasses)
            continue;

        const int G      = out.size[2];
        const int stride = S / G;
        const float *data = reinterpret_cast<const float *>(out.data);

        for (int gy = 0; gy < G; ++gy)
        {
            for (int gx = 0; gx < G; ++gx)
            {
                const int idx = gy * G + gx;

                // --- 类别：80 个 logits，最大者即类别 ---
                const float *clsPtr = data + kBoxCh * G * G + idx;
                int   bestId    = 0;
                float bestScore = sigmoid(clsPtr[0]);
                for (int c = 1; c < kNumClasses; ++c)
                {
                    const float s = sigmoid(clsPtr[c * G * G]);
                    if (s > bestScore) { bestScore = s; bestId = c; }
                }
                if (bestScore < m_conf) continue;

                // --- 框：DFL，每个方向 16 个 bin 做 softmax 再求期望 ---
                float dist[4] = {0, 0, 0, 0};
                for (int k = 0; k < 4; ++k)
                {
                    float bins[kNumBins];
                    for (int b = 0; b < kNumBins; ++b)
                        bins[b] = data[(k * kNumBins + b) * G * G + idx];
                    softmax(bins, kNumBins);
                    for (int b = 0; b < kNumBins; ++b) dist[k] += b * bins[b];
                }

                // 左/上/右/下 到中心点的距离（单位：像素）
                const float l = dist[0] * stride;
                const float t = dist[1] * stride;
                const float r = dist[2] * stride;
                const float b = dist[3] * stride;

                const float cx = (gx + 0.5f) * stride;  // 格子中心
                const float cy = (gy + 0.5f) * stride;

                // letterbox 坐标 → 原图坐标
                const int x1 = int((cx - l - ox) / scale);
                const int y1 = int((cy - t - oy) / scale);
                const int x2 = int((cx + r - ox) / scale);
                const int y2 = int((cy + b - oy) / scale);

                const int X1 = std::max(0, std::min(x1, bgr.cols - 1));
                const int Y1 = std::max(0, std::min(y1, bgr.rows - 1));
                const int X2 = std::max(0, std::min(x2, bgr.cols - 1));
                const int Y2 = std::max(0, std::min(y2, bgr.rows - 1));
                if (X2 <= X1 || Y2 <= Y1) continue;

                boxes.emplace_back(cv::Rect(X1, Y1, X2 - X1, Y2 - Y1));
                scores.push_back(bestScore);
                classIds.push_back(bestId);
            }
        }
    }

    // 4) NMS 去重
    std::vector<int> keep;
    cv::dnn::NMSBoxes(boxes, scores, m_conf, m_nms, keep);

    result.reserve(keep.size());
    for (int i : keep)
        result.push_back({boxes[i], scores[i], classIds[i]});
    return result;
}

// 画绿框（侦测到人 → 画绿框）
void YoloDetector::drawGreenBoxes(cv::Mat &bgr, const std::vector<Detection> &dets)
{
    static const char *kNames[] = {
        "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train",
        "truck", "boat", "traffic light"
    };

    for (const Detection &d : dets)
    {
        if (d.classId != 0) continue;  // 只画“人”

        cv::rectangle(bgr, d.box, cv::Scalar(0, 255, 0), 3);

        const std::string label =
            std::string("person ") + std::to_string(d.score).substr(0, 4);
        cv::putText(bgr, label,
                    cv::Point(d.box.x, std::max(12, d.box.y - 6)),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
    }
}

int YoloDetector::countPersons(const std::vector<Detection> &dets)
{
    int n = 0;
    for (const Detection &d : dets)
        if (d.classId == 0) ++n;
    return n;
}
