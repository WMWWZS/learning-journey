#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QDateEdit>
#include <QTableView>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>
#include <QTabWidget>
#include <QLabel>
#include "playerwidget.h"
#include "../service/videoservice.h"

class VideoWidget : public QWidget {
    Q_OBJECT
public:
    //工具
    VideoFilter makeFilter(const QDate &day, int channelId)
    {
        VideoFilter f;
        f.channelId = channelId;
        const QString d = day.toString("yyyy-MM-dd");
        f.from = d + " 00:00:00";
        f.to   = day.addDays(1).toString("yyyy-MM-dd") + " 00:00:00";
        return f;  //拿到的是副本，原VideoFilter f摧毁
    }
    VideoWidget(QWidget *parent = nullptr);
private:
    //顶部
    QDateEdit *m_date = nullptr; //日期选择
    QComboBox *m_channel = nullptr;  //通道选择

    //左侧
    QListWidget *m_list = nullptr; //录像列表
    QPushButton *m_btnNext = nullptr;  //下一页
    QPushButton *m_btnPrev = nullptr;  //上一页
    QPushButton *m_btnFirst = nullptr; //首页
    QPushButton *m_btnLast = nullptr;  //末页
    QLabel* m_pageLabel = nullptr; // 页码显示 "2/10"

    //右侧
    QTabWidget *m_tabs = nullptr;   //视频预览 图片预览
    PlayerWidget *m_player = nullptr; //播放器
    QPushButton *m_btnShot = nullptr;  //截屏
    QComboBox *m_speed = nullptr; //倍速

    VideoService *m_service = nullptr; //业务层，负责查数据

    int m_page = 1; //页码从1开始
    int m_pageSize = 10; //每页10条
    void loadPage(int page);
    void initConnect();
private slots:
    //---翻页
    void onBtnFirstClicked();  //首页
    void onBtnPrevClicked();   //上一页
    void onBtnNextClicked();    //下一页
    void onBtnLastClicked();     //最后
    //---数据变化
    void onFilerChange();  //日期或者通道改变

    //---播放器状态
    void onShotClicked(); //截屏
    void onSpeedChanged(int index);  //速度选择
};

#endif // VIDEOWIDGET_H
