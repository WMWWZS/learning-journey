#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QDebug>

using namespace cv;
using namespace dnn;
using namespace std;



void test01()
{
    //1.加载数据模型
    CascadeClassifier face_cascade;
    if(!face_cascade.load("haarcascade_frontalface_default.xml"))
    {
        qDebug() << "数据模型加载失败";
        return;
    }

    //2.图像预处理
    // Mat img = imread("../image/BHJBHCDAAJJJC-hs7xXPRDhg.png");
    Mat img = imread("../image/BHJBHCDCEICFJ-xRJg585Kna.png");
    Mat gray;

    cvtColor(img, gray, COLOR_BGR2GRAY);

    //3.调用检测方法
    std::vector<Rect> faces;   // 用于保存人脸的坐标信息
    face_cascade.detectMultiScale(
        gray,   //待检测的目标
        faces,  //检测到的目标数据
        1.1, //缩放系数，越小检测越精准，速度越慢
        5,  //最小邻域值
        0,
        Size(30, 30));  // 忽略30*30的目标

    //4.画框标记
    for(const auto&f : faces)
    {
        rectangle(img, f, Scalar(0,255,0),2);
    }

    //5.显示
    namedWindow("dected",  WINDOW_NORMAL);
    imshow("dected", img);
    waitKey(0);
}


class YOLOv8_face
{
public:
    YOLOv8_face(string modelpath, float confThreshold, float nmsThreshold);
    void detect(Mat& frame);
private:
    Mat resize_image(Mat srcimg, int *newh, int *neww, int *padh, int *padw);
    const bool keep_ratio = true;  //resize_image() 里判断；为 true 时等比缩放+补边，不会把人脸拉变形
    const int inpWidth = 640;
    const int inpHeight = 640;
    float confThreshold;   //置信度阈值 低于这个值将直接放弃
    float nmsThreshold;   //非极大值抑制阈值 两个人指同一张脸时，只留一个。重复的框（重叠太多）合并掉
    const int num_class = 1;  ///只有人脸这一个类别
    const int reg_max = 16;
    Net net;
    void softmax_(const float* x, float* y, int length);
    void generate_proposal(Mat out, vector<Rect>& boxes, vector<float>& confidences, vector< vector<Point>>& landmarks, int imgh, int imgw, float ratioh, float ratiow, int padh, int padw);
    void drawPred(float conf, int left, int top, int right, int bottom, Mat& frame, vector<Point> landmark);
};

static inline float sigmoid_x(float x)
{
    return static_cast<float>(1.f / (1.f + exp(-x)));
}

YOLOv8_face::YOLOv8_face(string modelpath, float confThreshold, float nmsThreshold)
{
    this->confThreshold = confThreshold;
    this->nmsThreshold = nmsThreshold;
    this->net = readNet(modelpath);
}

Mat YOLOv8_face::resize_image(Mat srcimg, int *newh, int *neww, int *padh, int *padw)
{
    int srch = srcimg.rows, srcw = srcimg.cols;
    *newh = this->inpHeight;
    *neww = this->inpWidth;
    Mat dstimg;
    if (this->keep_ratio && srch != srcw) {
        float hw_scale = (float)srch / srcw;
        if (hw_scale > 1) {
            *newh = this->inpHeight;
            *neww = int(this->inpWidth / hw_scale);
            resize(srcimg, dstimg, Size(*neww, *newh), INTER_AREA);
            *padw = int((this->inpWidth - *neww) * 0.5);
            copyMakeBorder(dstimg, dstimg, 0, 0, *padw, this->inpWidth - *neww - *padw, BORDER_CONSTANT, 0);
        }
        else {
            *newh = (int)this->inpHeight * hw_scale;
            *neww = this->inpWidth;
            resize(srcimg, dstimg, Size(*neww, *newh), INTER_AREA);
            *padh = (int)(this->inpHeight - *newh) * 0.5;
            copyMakeBorder(dstimg, dstimg, *padh, this->inpHeight - *newh - *padh, 0, 0, BORDER_CONSTANT, 0);
        }
    }
    else {
        resize(srcimg, dstimg, Size(*neww, *newh), INTER_AREA);
    }
    return dstimg;
}

void YOLOv8_face::drawPred(float conf, int left, int top, int right, int bottom, Mat& frame, vector<Point> landmark)   // Draw the predicted bounding box
{
    //Draw a rectangle displaying the bounding box
    rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 0, 255), 3);

    //Get the label for the class name and its confidence
    string label = format("face:%.2f", conf);

    //Display the label at the top of the bounding box
    /*int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = max(top, labelSize.height);
    rectangle(frame, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);*/
    putText(frame, label, Point(left, top-5), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
    for (int i = 0; i < 5; i++)
    {
        circle(frame, landmark[i], 4, Scalar(0, 255, 0), -1);
    }
}

void YOLOv8_face::softmax_(const float* x, float* y, int length)
{
    float sum = 0;
    int i = 0;
    for (i = 0; i < length; i++)
    {
        y[i] = exp(x[i]);
        sum += y[i];
    }
    for (i = 0; i < length; i++)
    {
        y[i] /= sum;
    }
}

void YOLOv8_face::generate_proposal(Mat out, vector<Rect>& boxes, vector<float>& confidences, vector< vector<Point>>& landmarks, int imgh,int imgw, float ratioh, float ratiow, int padh, int padw)
{
    const int feat_h = out.size[2];
    const int feat_w = out.size[3];
    cout << out.size[1] << "," << out.size[2] << "," << out.size[3] << endl;
    const int stride = (int)ceil((float)inpHeight / feat_h);
    const int area = feat_h * feat_w;
    float* ptr = (float*)out.data;
    float* ptr_cls = ptr + area * reg_max * 4;
    float* ptr_kp = ptr + area * (reg_max * 4 + num_class);

    for (int i = 0; i < feat_h; i++)
    {
        for (int j = 0; j < feat_w; j++)
        {
            const int index = i * feat_w + j;
            int cls_id = -1;
            float max_conf = -10000;
            for (int k = 0; k < num_class; k++)
            {
                float conf = ptr_cls[k*area + index];
                if (conf > max_conf)
                {
                    max_conf = conf;
                    cls_id = k;
                }
            }
            float box_prob = sigmoid_x(max_conf);
            if (box_prob > this->confThreshold)
            {
                float pred_ltrb[4];
                float* dfl_value = new float[reg_max];
                float* dfl_softmax = new float[reg_max];
                for (int k = 0; k < 4; k++)
                {
                    for (int n = 0; n < reg_max; n++)
                    {
                        dfl_value[n] = ptr[(k*reg_max + n)*area + index];
                    }
                    softmax_(dfl_value, dfl_softmax, reg_max);

                    float dis = 0.f;
                    for (int n = 0; n < reg_max; n++)
                    {
                        dis += n * dfl_softmax[n];
                    }

                    pred_ltrb[k] = dis * stride;
                }
                float cx = (j + 0.5f)*stride;
                float cy = (i + 0.5f)*stride;
                float xmin = max((cx - pred_ltrb[0] - padw)*ratiow, 0.f);  ///还原回到原图
                float ymin = max((cy - pred_ltrb[1] - padh)*ratioh, 0.f);
                float xmax = min((cx + pred_ltrb[2] - padw)*ratiow, float(imgw - 1));
                float ymax = min((cy + pred_ltrb[3] - padh)*ratioh, float(imgh - 1));
                Rect box = Rect(int(xmin), int(ymin), int(xmax - xmin), int(ymax - ymin));
                boxes.push_back(box);
                confidences.push_back(box_prob);

                vector<Point> kpts(5);
                for (int k = 0; k < 5; k++)
                {
                    float x = ((ptr_kp[(k * 3)*area + index] * 2 + j)*stride - padw)*ratiow;  ///还原回到原图
                    float y = ((ptr_kp[(k * 3 + 1)*area + index] * 2 + i)*stride - padh)*ratioh;
                    ///float pt_conf = sigmoid_x(ptr_kp[(k * 3 + 2)*area + index]);
                    kpts[k] = Point(int(x), int(y));
                }
                landmarks.push_back(kpts);
            }
        }
    }
}


void YOLOv8_face::detect(Mat& srcimg)
{
    int newh = 0, neww = 0, padh = 0, padw = 0;
    Mat dst = this->resize_image(srcimg, &newh, &neww, &padh, &padw);  //先等比例缩小，得到 640×640 的标准照片
    Mat blob;
    blobFromImage(dst, blob, 1 / 255.0, Size(this->inpWidth, this->inpHeight), Scalar(0, 0, 0), true, false); // 把像素从 0~255 压到 0~1，BGR 转 RGB
    this->net.setInput(blob);
    vector<Mat> outs;
    ///net.enableWinograd(false);  ////如果是opencv4.7，那就需要加上这一行
    this->net.forward(outs, this->net.getUnconnectedOutLayersNames());  //进行推理

    /////generate proposals
    vector<Rect> boxes;
    vector<float> confidences;
    vector< vector<Point>> landmarks;
    float ratioh = (float)srcimg.rows / newh, ratiow = (float)srcimg.cols / neww;

    generate_proposal(outs[0], boxes, confidences, landmarks, srcimg.rows, srcimg.cols, ratioh, ratiow, padh, padw);
    generate_proposal(outs[1], boxes, confidences, landmarks, srcimg.rows, srcimg.cols, ratioh, ratiow, padh, padw);
    generate_proposal(outs[2], boxes, confidences, landmarks, srcimg.rows, srcimg.cols, ratioh, ratiow, padh, padw);

    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    vector<int> indices;
    NMSBoxes(boxes, confidences, this->confThreshold, this->nmsThreshold, indices);  //去重
    for (size_t i = 0; i < indices.size(); ++i)  //画图
    {
        int idx = indices[i];
        Rect box = boxes[idx];
        this->drawPred(confidences[idx], box.x, box.y,
                       box.x + box.width, box.y + box.height, srcimg, landmarks[idx]);
    }
}

int main()
{
    YOLOv8_face YOLOv8_face_model("weights/yolov8n-face.onnx", 0.45, 0.5); // 把握不到 45%的候选框不要 两个框重叠超过 50% 就认为是同一张脸
    string imgpath = "image/BHJBHCDCEICFJ-xRJg585Kna.png";
    Mat srcimg = imread(imgpath);
    YOLOv8_face_model.detect(srcimg);

    static const string kWinName = "Deep learning face detection use OpenCV";
    namedWindow(kWinName, WINDOW_NORMAL);
    imshow(kWinName, srcimg);
    waitKey(0);
    destroyAllWindows();
}
