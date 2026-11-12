#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include "verifycodelabel.h"
class loginWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton* btn_login;
    QLineEdit* nameEdit;
    QLineEdit* pwdEdit;
public:
    explicit loginWidget(QWidget *parent = nullptr);
    void init_qss();
    void init_connect();
signals:   //自定义信号区，只需要声明，不需要定义
    void login_success(QString);
public slots:
    void login();
};

#endif // LOGINWIDGET_H
