#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QPixmap>
#include <QPalette>
#include <QDebug>
#include <QFile>
#include "verifycodelabel.h"
void init_qss(QWidget* qw)
{
    QFile file(":/qss/login_reg.qss");
    if(file.open(QFile::ReadOnly))
    {
        qw->setStyleSheet(file.readAll());
        file.close();
    }

}

void test04()
{

    QWidget* mainWindow = new QWidget;
    mainWindow->setWindowTitle("注册界面测试");
    mainWindow->resize(800, 600);

    QLabel* lab_title = new QLabel("注册系统测试");
    lab_title->setFont(QFont("微软雅黑", 20, QFont::Bold));
    lab_title->setAlignment(Qt::AlignCenter);  //居中


    //设置背景图片
    QPixmap pm("./images/BG3.jpg");
    QPalette pal = mainWindow->palette();
    pal.setBrush(QPalette::Window,QBrush(pm.scaled(mainWindow->size())));
    mainWindow->setAutoFillBackground(true);
    mainWindow->setPalette(pal);
    if(pm.isNull())
    {
        qDebug()<<"图片加载失败";
    }
    else
    {
        qDebug()<<"图片加载成功";
    }

    //1.确定主布局
    QVBoxLayout* V_layout = new QVBoxLayout(mainWindow);   //个人错误：未加入mainWindow，也可以不加


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


    //-----------------------------
    //3.按钮布局
    QHBoxLayout* btnLayout = new QHBoxLayout;    //水平
    btnLayout->addStretch(1);
    btnLayout->addWidget(new QPushButton("注册"));
    btnLayout->addWidget(new QPushButton("取消"));
    btnLayout->addStretch(1);
    //-----------------------------


    //-----------------------------
    //4.添加进入主布局
    V_layout->addStretch();        // 上方弹簧，占掉上面多余空间

    V_layout->addWidget(lab_title);
    V_layout->addSpacing(70);      //标题和表单之间空隙
    V_layout->addLayout(formH);
    V_layout->addSpacing(30);      //表单和按钮之间空隙
    V_layout->addLayout(btnLayout);

    V_layout->addStretch();        //下方弹簧，占掉下面多余空间
    init_qss(mainWindow);
    mainWindow->show();
}

void test03()
{
    QWidget *window = new QWidget;
    window->setWindowTitle("登录界面测试");
    window->resize(800, 600);  //把这个窗口大小固定成 宽 300 像素，高 180 像素

    QLabel* lab_title = new QLabel("登录系统测试");
    lab_title->setFont(QFont("微软雅黑", 20, QFont::Bold));
    lab_title->setAlignment(Qt::AlignCenter);

    QPixmap pm("./images/BG3.jpg");
    QPalette pal = window->palette();
    pal.setBrush(QPalette::Window,QBrush(pm.scaled(window->size())));
    window->setAutoFillBackground(true);
    window->setPalette(pal);

    //1.确定主布局（垂直）
    QVBoxLayout* Vlay = new QVBoxLayout(window);


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

    init_qss(window);

    window->show();
}



void test01()
{
    // 1.垂直布局
    QWidget* pWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(pWidget);

    layout->addWidget(new QPushButton("按钮一"));
    layout->addWidget(new QPushButton("按钮二"));
    layout->addWidget(new QPushButton("按钮三"));

    pWidget->show();
}

void test02()
{
    // 2.水平布局
    QWidget* pWidget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(pWidget);

    layout->addWidget(new QPushButton("按钮一"));
    layout->addWidget(new QPushButton("按钮二"));
    layout->addWidget(new QPushButton("按钮三"));

    pWidget->show();
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // test01();
    //test02();
    // test03();
    test04();
    return QApplication::exec(); // 启动事件循环

}