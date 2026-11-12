#include "videowidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <QSqlQueryModel>

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent)
{
    m_service = new VideoService(this);
    //顶部：通道日期+通道 --------
    m_date = new QDateEdit(QDate::currentDate());
    m_date->setCalendarPopup(true); //点一下弹日历
    m_date->setDisplayFormat("yyyy/MM/dd");
    m_date->setFixedWidth(140);  //控件宽度

    m_channel = new QComboBox;
    m_channel->addItem("通道选择", 0);
    for (int i = 1; i <= 4; ++i)
    {
       m_channel->addItem(QString("通道 %1").arg(i), i); // itemData 存通道号
    }
    m_channel->setFixedWidth(140);


    QHBoxLayout *topBar = new QHBoxLayout;
    topBar->setContentsMargins(0, 0, 0, 0);   //布局不要留外边空白，紧贴边框，不留空隙
    topBar->addWidget(m_date);
    topBar->addWidget(m_channel);
    topBar->addStretch();

    //左侧：录像列表+翻页条-------
    m_list = new QListWidget;
    m_list->setMinimumWidth(230);
    m_list->setAlternatingRowColors(true); //隔行变色，好认
    m_btnPrev = new QPushButton("上一页");
    m_btnNext = new QPushButton("下一页");
    m_pageLabel = new QLabel("1/1");
    m_pageLabel->setAlignment(Qt::AlignCenter); //居中显示

    //页码
    QHBoxLayout *pager = new QHBoxLayout;
    pager->addWidget(m_btnPrev);
    pager->addWidget(m_pageLabel, 1);// 页码占中间，拉伸
    pager->addWidget(m_btnNext);

    QVBoxLayout *left = new QVBoxLayout;
    left->setContentsMargins(0, 0, 0, 0);
    left->addWidget(m_list, 1);                    // 列表占满剩余高度
    left->addLayout(pager);


    //右侧：视频预览 / 图片预览----------
    m_tabs = new QTabWidget;
    //视频页=播放器 + 底部操作栏
    QWidget *videoPage = new QWidget;
    QVBoxLayout *videoLayout = new QVBoxLayout(videoPage);
    videoLayout->setContentsMargins(0, 0, 0, 0);
    // m_player = new PlayerWidget;
    m_btnPause = new QPushButton("暂停");
    m_btnShot = new QPushButton("抓拍");
    m_speed = new QComboBox;
    m_speed->addItem("倍速", 1.0);   //倍速只是标签，选中就是1.0x
    m_speed->addItem("0.5x", 0.5);
    m_speed->addItem("1.0x", 1.0);
    m_speed->addItem("2.0x", 2.0);
    m_speed->addItem("4.0x", 4.0);
    m_speed->setFixedWidth(90);

    QHBoxLayout *actions = new QHBoxLayout;
    actions->addStretch();
    actions->addWidget(m_btnPause);
    actions->addWidget(m_btnShot);
    actions->addWidget(m_speed);
    actions->addStretch();  //三个按钮居中

    videoLayout->addWidget(m_player, 1);  //播放器占满
    videoLayout->addLayout(actions);      //操作栏在下面

    QWidget *imagePage = new QWidget;
    QVBoxLayout *imageLayout = new QVBoxLayout(imagePage);
    QLabel *imageHint = new QLabel("图片预览（下一步实现 3×3 缩略图）");
    imageHint->setAlignment(Qt::AlignCenter);
    imageLayout->addWidget(imageHint);

    m_tabs->addTab(videoPage, "视频预览");
    m_tabs->addTab(imagePage, "图片预览");
    m_tabs->setCurrentIndex(0);

    //左右拼接整体布局------
    QHBoxLayout *body = new QHBoxLayout;
    body->addLayout(left, 0);          //左列固定宽度由内部控件决定
    body->addWidget(m_tabs, 1);        //右侧占满
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topBar);
    mainLayout->addLayout(body, 1);

    initConnect();
    loadPage(1);

}


void VideoWidget::loadPage(int m_page)
{
    if(page < 1)
    {
        page = 1;
    }
    const VideoFilter f = makeFilter(m_date->date(), m_channel->currentData().toInt());
    const int totalPages = qMax(1, m_service->getFilteredPageCount(f, m_pageSize));
    if (page > totalPages) page = totalPages;
    m_page = page;
}


void VideoWidget::onFilerChange()
{
    loadPage(1);
}

void VideoWidget::onPauseClicked()
{
    m_player->pause();
}

void VideoWidget::onShotClicked()
{
    m_player->capture();
}

void VideoWidget::onSpeedChanged(int index)
{
    m_player->setSpeed(m_speed->itemData(index).toDouble());
}

void VideoWidget::initConnect()
{
    //翻页
    connect(m_btnFirst, &QPushButton::clicked, this, &VideoWidget::onBtnFirstClicked);
    connect(m_btnPrev, &QPushButton::clicked, this, &VideoWidget::onBtnPrevClicked);
    connect(m_btnNext, &QPushButton::clicked, this, &VideoWidget::onBtnNextClicked);
    connect(m_btnLast, &QPushButton::clicked, this, &VideoWidget::onBtnLastClicked);

    //日期
    connect(m_date, &QDateEdit::dateChanged, this, &VideoWidget::onFilerChange);
    connect(m_channel, &QComboBox::currentIndexChanged,this, &VideoWidget::onFilterChange);

    // 暂停/抓拍/倍速
    connect(m_btnPause, &QPushButton::clicked, this, &VideoWidget::onPauseClicked);
    connect(m_btnShot,  &QPushButton::clicked, this, &VideoWidget::onShotClicked);
    connect(m_speed, &QComboBox::currentIndexChanged,this, &VideoWidget::onSpeedChanged);

    //双击列表进行播放
    connect(m_list, &QListWidget::activated, this, [this](QListWidgetItem* item){
        if(item)
        {
            return;
        }
        const QString path = item->data(Qt::UserRole).toString();
        const int ch = item->data(Qt::UserRole + 1).toInt();
        if (path.isEmpty())
        {
            QMessageBox::warning(this, "回放", "该记录没有视频路径");
            return;
        }
        m_player->openFile(path, ch);
    });
    connect(m_player, &PlayerWidget::sig_error, this,
            [this](const QString &msg)
            {
        QMessageBox::warning(this, "回放", msg);  //弹窗是警告框，弹出后，你不能点后面的父窗口，必须先关掉弹窗

    });


}

void VideoWidget::onBtnFirstClicked()
{
    loadPage(1);
}

void VideoWidget::onBtnPrevClicked()
{
    loadPage(m_page - 1);
}

void VideoWidget::onBtnNextClicked()
{
    loadPage(m_page + 1);
}

void VideoWidget::onBtnLastClicked()
{
    const VideoFilter f = makeFilter(m_date->date(), m_channel->currentData().toInt());
    loadPage(m_service->getFilteredPageCount(f, m_pageSize));
}
