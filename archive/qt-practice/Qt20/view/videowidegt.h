#ifndef VIDEOWIDEGT_H
#define VIDEOWIDEGT_H


#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include "../service/videoservice.h"

class VideoWidget : public QWidget {
    Q_OBJECT
private:
    QTableView *videoTableView;
    VideoService *videoService;
    QPushButton *btnFirst, *btnPrev, *btnNext, *btnLast;
    QLabel *labelPage;
    int currentPage;
    int pageSize;

    void initVideoReview();
    void loadVideoPage(int page);
private slots:
    void onBtnFirstClicked();
    void onBtnPrevClicked();
    void onBtnNextClicked();
    void onBtnLastClicked();
    void onTableDoubleClicked(QModelIndex index);

};

#endif // VIDEOWIDEGT_H
