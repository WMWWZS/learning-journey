#include "loginwidget.h"

loginWidget::loginWidget(QWidget *parent): QWidget(parent)
{
    this->setWindowTitle("登录界面测试");
    this->resize(800, 600);
    // 固定窗口大小，禁止拖拽缩放
    this->setFixedSize(800, 600);

    QLabel* lab_title = new QLabel("登录系统测试", this);
    lab_title->setFont(QFont("微软雅黑", 20, QFont::Bold));
    lab_title->setAlignment(Qt::AlignCenter);

    QPixmap pm(":/imge/BG3.jpg");
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window,QBrush(pm.scaled(this->size())));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //1.确定主布局（垂直）
    QVBoxLayout* Vlay = new QVBoxLayout(this);

    //-----------------------------
    //2.表单布局
    QFormLayout* Flay = new QFormLayout;
    nameEdit = new QLineEdit;  //用户名
    pwdEdit = new QLineEdit;  //密码

    pwdEdit->setEchoMode(QLineEdit::Password);
    Flay->setVerticalSpacing(25);
    Flay->addRow("用户名：",nameEdit);
    Flay->addRow("密码：",pwdEdit);

    //表单居中
    QHBoxLayout* formH = new QHBoxLayout;
    formH->addStretch();
    formH->addLayout(Flay);
    formH->addStretch();

    //验证码布局
    QHBoxLayout* vcode_layout = new QHBoxLayout;
    QLabel* lab_vcode = new QLabel("验证码：");
    QLineEdit* vcode_edit = new QLineEdit;
    VerifyCodeLabel* pic_vcode = new VerifyCodeLabel;
    vcode_layout->addWidget(vcode_edit);
    vcode_layout->addWidget(pic_vcode);
    Flay->addRow(lab_vcode, vcode_layout);

    //3.按钮布局
    QHBoxLayout* btnLayout = new QHBoxLayout;    //水平

    btn_login = new QPushButton("登录");
    QPushButton* btn_reg = new QPushButton("注册");
    QPushButton* btn_cancel = new QPushButton("取消");

    btnLayout->addStretch();
    btnLayout->addWidget(btn_login);
    btnLayout->addWidget(btn_reg);
    btnLayout->addWidget(btn_cancel);
    btnLayout->addStretch();

    //4.组装 提交给主布局进行管理
    Vlay->addStretch();
    Vlay->addWidget(lab_title);
    Vlay->addSpacing(65);
    Vlay->addLayout(formH);
    Vlay->addSpacing(25);
    Vlay->addLayout(btnLayout);
    Vlay->addStretch();

    init_qss();

    init_connect();
}

void loginWidget::init_qss()
{
    QFile file(":/qss/login_reg.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void loginWidget::init_connect()
{
    connect(btn_login,SIGNAL(clicked()), this, SLOT(login()));
}

void loginWidget::login()
{
    //登录判断逻辑验证成功——>发送成功信号——>
    QString userName = nameEdit->text().trimmed();
    emit login_success(userName);
}

