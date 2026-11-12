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
    QLineEdit* nameEdit = new QLineEdit;
    QLineEdit* pwdEdit = new QLineEdit;
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
    btnLayout->addStretch();
    btnLayout->addWidget(new QPushButton("登录"));
    btnLayout->addWidget(new QPushButton("注册"));
    btnLayout->addWidget(new QPushButton("取消"));
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
    //删掉 window->show(); 构造函数结束自动显示

}

void loginWidget::init_qss()
{
    QFile file(":/qss/login_reg.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

