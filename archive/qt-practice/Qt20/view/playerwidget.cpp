#include "playerwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QThread>
#include <QtMinMax>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QSqlQuery>
#include <QMetaObject>
#include "service/playbacktask.h"
#include "util/dbconn.h"

PlayerWidget::PlayerWidget(QWidget *parent) : QWidget{parent}
{
    //---画面区---
    m_view = new QLabel;
    m_view->setMinimumSize(480, 270);
    m_view->setAlignment(Qt::AlignCenter); //居中显示
    m_view->setStyleSheet("background:# 1e1e1e; color:#888;");
    m_view->setText("视频将在预览时播放");

    //---控制条---
    m_btnPlay = new QPushButton("▶");

    m_timeLabel = new QLabel("0:00/0:00");
    m_timeLabel->setMinimumWidth(90); //最小宽度90，可以变大 长不限制

    m_slider = new QSlider(Qt::Horizontal); //水平方向的进度条

    m_btnMute = new QPushButton("🔊");
    m_btnMute->setFixedWidth(36);  //固定宽度，不能变大变小

    m_btnFull = new QPushButton("⛶");
    m_btnFull->setFixedWidth(36); //固定宽度，不能变大变小

    //---横向排列---
    QHBoxLayout *ctrl = new QHBoxLayout;
    ctrl->setContentsMargins(8, 2, 8, 2);  //布局和它外层容器之间的边距
    ctrl->setSpacing(8);
    ctrl->addWidget(m_btnPlay);
    ctrl->addWidget(m_timeLabel);
    ctrl->addWidget(m_slider, 1);   // 第二个参数1拉伸，进度条吃掉多余宽度
    ctrl->addWidget(m_btnMute);
    ctrl->addWidget(m_btnFull);


    //整体：画面在上，控制条在下
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_view, 1);
    layout->addLayout(ctrl);

    //---解码任务和线程---
    m_task = new PlaybackTask;
    m_thread = new QThread(this);
    m_task->moveToThread(m_thread);
    //线程启动
    connect(m_thread, &QThread::started, m_task, &PlaybackTask::startDecode);
    //---解码线程相关的信号链接---
    connect(m_task, &PlaybackTask::frameReady, this, &PlayerWidget::onFrameReady);
    connect(m_task, &PlaybackTask::opened, this, &PlayerWidget::onOpened);//就是说path路径的视频已经分析差不多了，传了个fps和总帧数用于设置滑块、
    connect(m_task, &PlaybackTask::positionChanged, this, &PlayerWidget::onPositionChanged);
    connect(m_task, &PlaybackTask::finished, this, &PlayerWidget::onFinished);
    connect(m_task, &PlaybackTask::error, this, &PlayerWidget::sig_error);
    //---用户界面控件的信号连接---
    connect(m_btnPlay, &QPushButton::clicked, this, &PlayerWidget::onPlayPause);
    connect(m_btnMute, &QPushButton::clicked, this, &PlayerWidget::onMuteClicked);
    connect(m_btnFull, &QPushButton::clicked, this, &PlayerWidget::onFullscreenClicked);
    connect(m_slider, &QSlider::sliderPressed, this, &PlayerWidget::onSliderPressed);
    connect(m_slider, &QSlider::sliderReleased, this, &PlayerWidget::onSliderReleased);
}

PlayerWidget::~PlayerWidget()
{
    if(m_thread && m_task && m_thread->isRunning())
    {
        /*
        第三个参数
        同样把事件丢进接收者线程的队列，但是，
        发送线程卡住不动，阻塞等待，直到槽函数执行完毕，才返回
        */
        QMetaObject::invokeMethod(m_task, "stop", Qt::BlockingQueuedConnection);//注意stop得是槽函数也可以是Q_INVOKABLE标记的普通函数
        m_thread->quit();
        m_thread->wait(3000);
    }
}

bool PlayerWidget::openFile(const QString &path, int channelId)
{
    m_channelId = channelId;

    QFileInfo fi(path); //转成绝对路径
    QString abs = path;
    if(!fi.isAbsolute()) //判断是不是绝对路径
    {
        const QString candidate =QDir(QCoreApplication::applicationDirPath()).filePath(path);
        //2.判断这个候选路径对应的文件是不是真的存在
        if (QFileInfo::exists(candidate))
        {
            //3.存在采用这个候选路径
            abs = candidate;
        }
        //如果候选文件不存在：abs维持原来的path，不修改
    }

    if (!m_thread->isRunning())
    {
        m_task->setSource(abs);     //在启动线程前设置路径是安全的
        m_thread->start();          //started -> PlaybackTask::startDecode
    }
    else
    {
        QMetaObject::invokeMethod(m_task, "open", Qt::QueuedConnection,
                                  Q_ARG(QString, abs));
    }
    //状态复位
    m_playing = true;
    m_finished = false;
    m_lastImage = QImage();
    updatePlayButton();
    return true;
}

//暂停
void PlayerWidget::pause()
{
    if(!m_playing)
    {
        return;
    }
    QMetaObject::invokeMethod(m_task, "pause", Qt::QueuedConnection);
    m_playing = false;
    updatePlayButton();
}

void PlayerWidget::capture()
{
    if(m_lastImage.isNull())
    {
        emit sig_error("当前没有画面可抓拍");
        return;
    }
    //目录：<exe目录>/features/manual/
    QDir().mkpath(QDir(QCoreApplication::applicationDirPath())
                      .filePath("features/manual"));

    const QString name = QString("手动_ch%1_%2.jpg")
                             .arg(m_channelId)
                             .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));

    const QString absPath = QDir(QCoreApplication::applicationDirPath()).filePath("features/manual"+name);

    //图片保存
    if (!m_lastImage.save(absPath, "JPG", 92))  //第三个是文件的质量类似分辨率把
    {
        emit sig_error(QString("截图保存失败：%1").arg(absPath));
        return;
    }

    //写库：image_path 存相对路径，和 video 表保持一致
    QSqlQuery query(DbConn::getInstance()->getDb());
    QString sql = "insert into feature_image"
                  "(image_name, image_path, channel_id, capture_time, create_time) "
                  "VALUES(:name, :path, :ch, :cap, :create)";
    query.prepare(sql);
    const QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    query.bindValue(":name", name);
    query.bindValue(":path", QDir(QCoreApplication::applicationDirPath())
                                 .relativeFilePath(absPath)); //相对路径
    query.bindValue(":ch", m_channelId);
    query.bindValue(":cap", now);
    query.bindValue(":create", now);
    if (!query.exec())
    {
        emit sig_error(QString("截图入库失败：%1").arg(query.lastError().text()));
    }
}

void PlayerWidget::setSpeed(double speed)
{
    QMetaObject::invokeMethod(m_task, "setSpeed", Qt::QueuedConnection, Q_ARG(double, speed));
    //我为什么不用Qt::DirectConnection？因为m_task有m_cap 原子标记 条件变量UI线程直接进去调用会数据竞争崩溃十分严重
    /*
        Qt::QueuedConnection就是异步，打包成事件，丢进目标对象（m_task）的事件循环，等它有空再执行
        调用方（UI 线程）发完立刻返回，不等函数跑完
        Qt::BlockingQueuedConnection 阻塞队列，打包事件丢给目标线程，然后调用方卡住，等对方执行完才返回
    */
}

//播放和暂停的切换
void PlayerWidget::onPlayPause()
{
    if(!m_thread)  //如果线程未开启
    {
        return;
    }
    if(m_playing)  //如果正在播放
    {
        QMetaObject::invokeMethod(m_task, "pause", Qt::QueuedConnection); //Qt不能跨线程直接调用别的线程对象的成员函数，所以用这个
        m_playing = false; //暂停状态
    }
    else
    {
        if(m_finished) //如果播放完成
        {
            QMetaObject::invokeMethod(m_task, "seekTo", Qt::QueuedConnection, Q_ARG(int, 0));//跨线程调用和传参数给seekTo
            m_finished = false;
        }
        QMetaObject::invokeMethod(m_task, "play", Qt::QueuedConnection);
        m_playing = true;
    }
    updatePlayButton();
}

void PlayerWidget::onSliderPressed()
{
    m_dragging = true;   //拖拽期间不响应 positionChanged
}

//滑块松开
void PlayerWidget::onSliderReleased()
{
    m_dragging = false;
    QMetaObject::invokeMethod(m_task, "seekTo", Qt::QueuedConnection,
                              Q_ARG(int, m_slider->value()));
    refreshTimeLabel();
}


void PlayerWidget::onMuteClicked()
{
    // OpenCV 不解析音频，这里只是图标状态切换
    m_muted = !m_muted;
    m_btnMute->setText(m_muted ? "🔇" : "🔊");
}

void PlayerWidget::onFullscreenClicked()
{
    QWidget* win = window();
    if(win->isFullScreen()) //如果已经是全屏
    {
        win->showNormal();
        m_btnFull->setText("⛶");
    }
    else
    {
        win->showFullScreen();
        m_btnFull->setText("⛉");
    }
}

void PlayerWidget::onFrameReady(const QImage &img)
{
    if(img.isNull())
    {
        return;
    }
    const QSize target = m_view->contentsRect().size(); //拿到label里画图矩形的size
    if(!target.isEmpty())
    {
        m_view->setPixmap(QPixmap::fromImage(img).scaled(target,
                                                         Qt::KeepAspectRatio,
                                                         Qt::FastTransformation));  //.scaled(target,模式1,模式2)
    }

}

//设置滑块范围和时间
void PlayerWidget::onOpened(double fps, int totalFrames)
{
    m_fps = (fps > 1) ? fps : 25.0;
    m_totalFrames = qMax(1, totalFrames);
    //这里
    m_slider->setRange(0, m_totalFrames - 1); //为什么-1呢？因为就是假设视频100帧，那么帧编号就是0-99，100会直接越界
    m_slider->setValue(0);//初始帧
    refreshTimeLabel(); //更新
}

void PlayerWidget::onPositionChanged(int frameNo)
{
    if(m_dragging)
    {
        return;
    }
    m_slider->setValue(qBound(0, frameNo, qMax(0, m_totalFrames - 1)));
    refreshTimeLabel();
}

void PlayerWidget::onFinished()
{
    m_playing = false;
    m_finished = true;
    updatePlayButton();
}

//更新时间文字，当前帧/总帧数
void PlayerWidget::refreshTimeLabel()  //帧数÷帧率=时间
{
    auto fmt = [](int sec){
        return QString("%1:%2")
            .arg(sec / 60)
            .arg(sec % 60, 2, 10, QChar('0')); //sec % 60余数（秒）开头，两位数，10进制，不够2位数用0填充
    }; //return回调用lambda,也就是调用refreshTimeLabel()的函数
}

void PlayerWidget::updatePlayButton()
{
    m_btnPlay->setText(m_playing ? "⏸" : "▶");
}

