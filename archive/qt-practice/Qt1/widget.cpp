#include "widget.h"
#include "./ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("监控系统");
    this->setFixedSize(800, 600);
    this->setFont(QFont("宋体", 12, 600));


    QLabel* name_user = new QLabel("用户名:", this);
    name_user->setGeometry(220, 160, 70, 30);

    QLineEdit* name_edi = new QLineEdit(this);
    name_edi->setGeometry(300, 160, 200, 30);


    QLabel* pwd_user = new QLabel("密码:", this);
    pwd_user->setGeometry(220, 220, 70, 30);

    QLineEdit* pwd_edi = new QLineEdit(this);
    pwd_edi->setGeometry(300, 220, 200, 30);
    pwd_edi->setEchoMode(QLineEdit::Password);


    QPushButton* btn_login = new QPushButton("登录", this);
    btn_login->setGeometry(260, 300, 80, 35);

    QPushButton* btn_cancel = new QPushButton("取消", this);
    btn_cancel->setGeometry(400, 300, 80, 35);

    // QPalette pal = this->palette();   //当前窗口画笔
    // pal.setColor(QPalette::Window, QColor("#ffffff"));
    // this->setAutoFillBackground(true);
    // this->setPalette(pal);

}

Widget::~Widget()
{
    delete ui;
}
