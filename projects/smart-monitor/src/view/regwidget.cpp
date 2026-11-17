#include "regwidget.h"

RegWidget::RegWidget(QWidget *parent) : QWidget{parent}
{

    this->setWindowTitle("注册界面测试");
    this->resize(800, 600);
    QLabel* lab_title = new QLabel("注册系统测试");
    lab_title->setFont(QFont("微软雅黑", 20, QFont::Bold));
    lab_title->setAlignment(Qt::AlignCenter);  //居中

    QPixmap pm(":/image/BG3.jpg");
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window,QBrush(pm.scaled(this->size())));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //1.确定主布局
    QVBoxLayout* V_layout = new QVBoxLayout(this);

    //-----------------------------
    //2.表单布局
    QFormLayout* F_layout = new QFormLayout;
    QLineEdit* nameEdit = new QLineEdit;
    QLineEdit* pwdEdit = new QLineEdit;
    QLineEdit* v_pwdEdit = new QLineEdit;

    F_layout->setVerticalSpacing(25); //表单之间的间距
    F_layout->addRow("用户名：", nameEdit);
    F_layout->addRow("密码：", pwdEdit);
    F_layout->addRow("确认密码：", v_pwdEdit);

    pwdEdit->setEchoMode(QLineEdit::Password);
    v_pwdEdit->setEchoMode(QLineEdit::Password);

    //表单居中
    QHBoxLayout* formH = new QHBoxLayout;
    formH->addStretch();
    formH->addLayout(F_layout);
    formH->addStretch();

    //-----------------------------
    //3.按钮布局
    QHBoxLayout* btnLayout = new QHBoxLayout; //水平
    btnLayout->addStretch(1);
    btnLayout->addWidget(new QPushButton("注册"));
    btnLayout->addWidget(new QPushButton("取消"));
    btnLayout->addStretch(1);

    //-----------------------------
    //4.添加进入主布局
    V_layout->addStretch();         // 上方弹簧，占掉上面多余空间
    V_layout->addWidget(lab_title);
    V_layout->addSpacing(70);       //标题和表单之间空隙
    V_layout->addLayout(formH);
    V_layout->addSpacing(30);       //表单和按钮之间空隙
    V_layout->addLayout(btnLayout);

    V_layout->addStretch();         // 下方弹簧，占掉下面多余空间

    init_qss();
}

void RegWidget::init_qss()
{
    QFile file(":/qss/login_reg.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}
