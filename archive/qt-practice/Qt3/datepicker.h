#ifndef DATEPICKER_H
#define DATEPICKER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCalendarWidget>
#include <QFrame>
#include <QDate>
#include <QDebug>
#include <QScreen>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDate>
#include <QGuiApplication>

class Datepicker : public QWidget
{
    Q_OBJECT
private:
    QLineEdit* m_lineEdit;       //日期显示输入框
    QPushButton* m_btn;          //切换日历控件显示隐藏的按钮
    QFrame* m_calendarFram;      //日历的容器控件
    QCalendarWidget* m_calendar; //日历控件
    QString m_dispalyFormat;     // 日期显示控件
public:
    explicit Datepicker(QWidget *parent = nullptr);
    void setDate(const QDate& date);     //设置日期
    QDate getDate() const;               //获取选中日期
    void setDisplayFormat(const QString& fromat);//设置日期显示格式
signals:
    void dateSeletcted(const QDate& date);  //日期被选择信号
protected:
    bool eventFilter(QObject* object, QEvent* event);   //事件过滤器
public slots:
    void onDateSelected(const QDate& date);   //日历控件中选中日期后的槽函数
    void toggleCalendarVisibility();
};

#endif // DATEPICKER_H
