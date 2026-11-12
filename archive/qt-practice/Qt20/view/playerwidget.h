#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <QString>
#include <opencv2/opencv.hpp>

/*
为什么要提前做类前声明？前向声明
只告诉编译器说我这个类存在，不用include，
仅用来定义指针进行占位，相关头文件在cpp文件里进行就好了
1.减少头文件依赖，加快编译速度
2.避免头文件循环包含
3.头文件更干净，隐藏内部实现
*/
class QLabel;
class QPushButton;
class QSlider;
class QTimer;
class QThread;
class PlaybackTask;
//  视频播放器：用PalybackTask进行解码任务，PlayerWidget负责接收图片
class PlayerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWidget(QWidget *parent = nullptr);
    ~PlayerWidget() override;
    bool openFile(const QString &path, int channelId = 0); //path是数据库里的path
    void pause(); //暂停
    void capture(); //截屏当前帧
    void setSpeed(double speed);  //倍速设置

signals:
    void sig_error(const QString &message);

private slots:
    //---用户界面操控相关---
    void onPlayPause();  //点播放
    void onSliderPressed();  //用户按住进度条
    void onSliderReleased();  // 用户松开进度条跳转
    void onMuteClicked();  //静音图标切换
    void onFullscreenClicked(); // 全屏切换

    //---解码线程送回来的信号处理---
    void onFrameReady(const QImage &img);//接收帧数用于显示
    void onOpened(double fps, int totalFrames);  //接收成功用于设置滑块范围
    void onPositionChanged(int frameNo); //当前帧号用于同步滑块
    void onFinished(); // 播完/停止用于恢复按钮
private:
    void refreshTimeLabel();   //更新0:00/0:00
    void updatePlayButton();  // 根据m_playing改按钮文字

    //---播放状态---
    QImage m_lastImage; //最近一帧，抓拍用
    int m_totalFrames = 0;
    int m_channelId = 0;
    double m_fps = 25.0; //视频帧率用来换算进度条
    double m_speed = 1.0;  //当前倍速
    bool m_dragging = false;   // 是否正在拖进度条（拖动时不同步位置
    bool m_playing = false; //是否正在播放 用来设置文本的
    bool m_muted = false;       // 静音标记（本播放器没有声音，仅图标状态）
    bool m_finished = false;

    //---界面控件---
    QLabel *m_view = nullptr;      // 画面
    QPushButton *m_btnPlay = nullptr;   //播放按钮
    QLabel *m_timeLabel = nullptr; // 时间文字
    QSlider *m_slider = nullptr;    // 进度条
    QPushButton *m_btnMute = nullptr;   // 静音
    QPushButton *m_btnFull = nullptr;   //全屏

    //---解码任务与线程---
    QThread *m_thread = nullptr;
    PlaybackTask *m_task= nullptr;
};

#endif // PLAYERWIDGET_H
