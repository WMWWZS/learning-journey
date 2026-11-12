#include "verifycodelabel.h"
#include <QPainter>
#include <QPen>
#include <QRandomGenerator>  // ★ Qt6 改用这个头文件

// 常量定义
const int LETTER_NUMBER = 4; // 验证码位数

VerifyCodeLabel::VerifyCodeLabel(QWidget *parent)
    : QLabel(parent), temp(true)
{
    // 设置固定大小
    setFixedSize(120, 40);
    // 设置鼠标手型，提示可点击
    setCursor(Qt::PointingHandCursor);

    // ★ Qt6 不需要手动初始化随机数种子，QRandomGenerator 自动处理

    // 初始化噪点数量（根据面积计算）
    noise_point_number = qMax(30, width() * height() * 3 / 100);
}

void VerifyCodeLabel::refreshCode()
{
    produceVerificationCode();  // 重新生成验证码字符串
    produceRandomColor();       // 重新生成字符颜色、噪点坐标
    update();                   // 触发paintEvent，重绘界面
}

void VerifyCodeLabel::produceVerificationCode()
{
    verificationCode.clear();
    QString chars = "ABCDEFGHJKLMNPQRSTUVWXYZabcdefghjkmnpqrstuvwxyz23456789"; // 去掉易混淆字符
    for (int i = 0; i < LETTER_NUMBER; ++i) {
        // ★ Qt6 写法：QRandomGenerator::global()->bounded(N) 生成 0 ~ N-1 的随机数
        int index = QRandomGenerator::global()->bounded(chars.length());
        verificationCode.append(chars.at(index));
    }
}

void VerifyCodeLabel::produceRandomColor()
{
    colorArray.clear();
    for (int i = 0; i < LETTER_NUMBER; ++i) {
        // 生成深色系，让文字更清晰
        QColor color(
            50 + QRandomGenerator::global()->bounded(130),   // 50 ~ 179
            50 + QRandomGenerator::global()->bounded(130),
            50 + QRandomGenerator::global()->bounded(130)
            );
        colorArray.append(color);
    }

    // 生成噪点坐标
    listX.clear();
    listY.clear();
    for (int i = 0; i < noise_point_number; ++i) {
        listX.append(QRandomGenerator::global()->bounded(width()));
        listY.append(QRandomGenerator::global()->bounded(height()));
    }
}

void VerifyCodeLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. 首次生成
    if (temp) {
        produceVerificationCode();
        produceRandomColor();
        temp = false;
    }

    // 2. 画背景
    painter.fillRect(rect(), QColor(240, 240, 240)); // 浅灰背景

    // 3. 画验证码字符
    int charWidth = rect().width() / LETTER_NUMBER;
    painter.setFont(QFont("Microsoft YaHei", 22, QFont::Bold));

    for (int i = 0; i < LETTER_NUMBER; ++i) {
        // 随机上下偏移
        int offsetY = QRandomGenerator::global()->bounded(11) - 5; // -5 ~ 5

        painter.setPen(colorArray.at(i));
        painter.drawText(
            i * charWidth + 5,
            rect().height() / 2 + offsetY + 10,
            QString(verificationCode[i])
            );
    }

    // 4. 画干扰线
    for (int i = 0; i < 3; ++i) {
        painter.setPen(QPen(QColor(
                                100 + QRandomGenerator::global()->bounded(101),  // 100 ~ 200
                                100 + QRandomGenerator::global()->bounded(101),
                                100 + QRandomGenerator::global()->bounded(101)
                                ), 1));
        painter.drawLine(
            QRandomGenerator::global()->bounded(width()),
            QRandomGenerator::global()->bounded(height()),
            QRandomGenerator::global()->bounded(width()),
            QRandomGenerator::global()->bounded(height())
            );
    }

    // 5. 画噪点
    painter.setPen(Qt::lightGray);
    for (int i = 0; i < noise_point_number; ++i) {
        painter.drawPoint(listX.at(i), listY.at(i));
    }
}

// 鼠标点击刷新验证码
void VerifyCodeLabel::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    temp = true; // 标记需要重新生成
    update();    // 触发重绘 (paintEvent)
}

QString VerifyCodeLabel::getVerificationCode() const
{
    return verificationCode;
}