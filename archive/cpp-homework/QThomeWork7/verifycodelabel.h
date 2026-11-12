#ifndef VERIFYCODELABEL_H
#define VERIFYCODELABEL_H

#include <QLabel>
#include <QColor>
#include <QList>

class VerifyCodeLabel : public QLabel
{
    Q_OBJECT
public:
    explicit VerifyCodeLabel(QWidget *parent = nullptr);

    // 获取当前验证码文本，用于登录校验
    QString getVerificationCode() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void produceVerificationCode(); // 生成随机字符串
    void produceRandomColor();      // 生成随机颜色

    QString verificationCode;       // 存储验证码文本
    QList<QColor> colorArray;       // 存储每个字符的颜色
    QList<int> listX;               // 噪点X坐标
    QList<int> listY;               // 噪点Y坐标
    int noise_point_number;         // 噪点数量

    bool temp; // 控制是否首次生成
};

#endif // VERIFYCODELABEL_H