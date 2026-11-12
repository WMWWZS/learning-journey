#include "Datepicker.h"

Datepicker::Datepicker(QWidget *parent) : QWidget{parent}
{
    this->m_dispalyFormat =  "yyyy-MM-dd";

    //1.输入框初始化
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setReadOnly(true);   //设置只读，用户不能键盘打字修改，只能通过日历弹窗选日期。
    m_lineEdit->setPlaceholderText("请选择日期");
    m_lineEdit->setCursor(Qt::PointingHandCursor);  //鼠标移动到输入框上，鼠标变成小手图标，提示可以点击。

    //2.按钮初始化
    m_btn = new QPushButton(this);
    m_btn->setText("...");

    //3.日历控件初始化QFrame
    m_calendarFram = new QFrame(nullptr,Qt::Tool | Qt::WindowStaysOnTopHint);  //工具窗口 置顶
    m_calendarFram->setFrameShape(QFrame::StyledPanel);    //跟随系统样式面板
    m_calendarFram->setFrameShadow(QFrame::Raised);     //凸起，看上去向外鼓
    m_calendarFram->setVisible(false);

    //4.日历控件初始化
    m_calendar = new QCalendarWidget(m_calendarFram);
    m_calendar->setGridVisible(true);   //日历显示格子网格线，每个日期格子有边框。
    m_calendar->setMinimumDate(QDate(1900,1,1));    //最小可选日期：1900‑01‑01，不能选更早
    m_calendar->setMaximumDate(QDate(2100,12,31));   //最大可选日期：2100‑12‑31，不能选更晚

    //5.设置默认显示日期
    setDate(QDate::currentDate());

    //6.布局初始化
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_lineEdit);
    mainLayout->addWidget(m_btn);
    mainLayout->setContentsMargins(0,0,0,0);

    QVBoxLayout *calendarLayout = new QVBoxLayout(m_calendarFram);
    calendarLayout->addWidget(m_calendar);
    calendarLayout->setContentsMargins(2,2,2,2);   //弹窗四周留 *2 像素边距

    m_lineEdit->installEventFilter(this);
    connect(m_btn, SIGNAL(clicked()), this, SLOT(toggleCalendarVisibility()));
    connect(m_calendar, SIGNAL(clicked(const QDate &)), this, SLOT(onDateSelected(const QDate &)));
}

void Datepicker::setDate(const QDate &date)
{
    if(date.isValid())
    {
        m_lineEdit->setText(date.toString(m_dispalyFormat));
        m_calendar->setSelectedDate(date);
    }
}

QDate Datepicker::getDate() const
{
    return m_calendar->selectedDate();
}

void Datepicker::setDisplayFormat(const QString &fromat)
{
    this->m_dispalyFormat = fromat;
}

bool Datepicker::eventFilter(QObject *object, QEvent *event)
{
    if(object == m_lineEdit && event->type() == QEvent::MouseButtonPress)
    {
        toggleCalendarVisibility();
        return true;
    }
    return QObject::eventFilter(object, event);
}

void Datepicker::onDateSelected(const QDate &date)
{
    setDate(date);
    m_calendarFram->setVisible(false);
    emit dateSeletcted(date);
}

void Datepicker::toggleCalendarVisibility()
{
    if(m_calendarFram->isVisible())
    {
        m_calendarFram->hide();
    }
    else
    {
        // 1、计算默认位置：输入框正下方
        QPoint pos = mapToGlobal(m_lineEdit->geometry().bottomLeft());

        // 2、强制刷新日历大小（避免太小看不到）
        m_calendarFram->adjustSize(); // 让框架自适应日历控件大小
        QSize calendarSize = m_calendarFram->size();

        // 3、屏幕边界检查，避免日历超出屏幕
        QRect screenRect = QGuiApplication::primaryScreen()->availableGeometry();
        // 若超出屏幕底部，显示在输入框上方
        if(pos.y() + calendarSize.height() > screenRect.bottom()){
            pos.setY(mapToGlobal(m_lineEdit->geometry().topLeft()).y() - calendarSize.height());
        }
        //若超出屏幕右侧，则左移对齐
        if(pos.x() + calendarSize.width() > screenRect.right()){
            pos.setX(screenRect.right() - calendarSize.width());
        }

        // 确保不超过屏幕左上角
        pos.setX(qMax(screenRect.left(),pos.x()));
        pos.setY(qMax(screenRect.top(),pos.y()));

        m_calendarFram->move(pos);
        m_calendarFram->show();
    }
}
