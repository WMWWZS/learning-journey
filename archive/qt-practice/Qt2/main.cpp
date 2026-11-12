#include "widget.h"
#include <QApplication>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QListWidgetItem>
#include <QListWidget>
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>


void test06()
{
    //优先级：控件级 > 窗口级 > 应用程序级
    QWidget* mainQ = new QWidget;
    mainQ->setWindowTitle("控件级demo");
    QVBoxLayout* main_L = new QVBoxLayout(mainQ);

    // //1.控件级
    QPushButton* tn1 = new QPushButton("这是控件级美化按钮", mainQ);
    QPushButton* tn2 = new QPushButton("按钮2", mainQ);
    QPushButton* tn3 = new QPushButton("按钮3", mainQ);
    QPushButton* tn4 = new QPushButton("按钮4", mainQ);
    QPushButton* tn5 = new QPushButton("按钮5", mainQ);
    QPushButton* tn6 = new QPushButton("按钮6", mainQ);

    // //2.窗口级
    // mainQ->setStyleSheet(R"(color:blue)");

    //别忘记分号
    // tn1->setStyleSheet(R"(
    // QPushButton{
    //     background:#0078d4;
    //     color:white;
    //     border:none;
    //     border-radius:6px;
    //     padding:8px;
    // }
    // QPushButton:hover{
    //         background:#3393dd;
    // }
// )");     //标准写法，R是为了防止转义


    main_L->addWidget(tn1);
    main_L->addWidget(tn2);
    main_L->addWidget(tn3);
    main_L->addWidget(tn4);
    main_L->addWidget(tn5);
    main_L->addWidget(tn6);

    mainQ->show();
}

void test05()
{
    QWidget* mainQ = new QWidget;
    mainQ->setWindowTitle("列表控件");
    mainQ->resize(400, 600);
    mainQ->setFont(QFont("宋体", 14, QFont::Bold));
    QVBoxLayout* mainLay = new QVBoxLayout(mainQ);

    QTableWidget* table = new QTableWidget(3, 3, mainQ);   //3行3列 交予mainQ管理

    //创建表头
    QStringList header = {"姓名", "职业", "年龄"};
    table->setHorizontalHeaderLabels(header);

    QTableWidgetItem* Item1 = new QTableWidgetItem("张三");
    QTableWidgetItem* Item2 = new QTableWidgetItem("工人");
    QTableWidgetItem* Item3 = new QTableWidgetItem("22");

    QTableWidgetItem* Item4 = new QTableWidgetItem("李四");
    QTableWidgetItem* Item5 = new QTableWidgetItem("务农");
    QTableWidgetItem* Item6 = new QTableWidgetItem("33");

    QTableWidgetItem* Item7 = new QTableWidgetItem("王五");
    QTableWidgetItem* Item8 = new QTableWidgetItem("铁路工人");
    QTableWidgetItem* Item9 = new QTableWidgetItem("22");

    table->setItem(0,0,Item1);
    table->setItem(0,1,Item2);
    table->setItem(0,2,Item3);     //当然，也可以直接new

    table->setItem(1,0,Item4);
    table->setItem(1,1,Item5);
    table->setItem(1,2,Item6);

    table->setItem(2,0,Item7);
    table->setItem(2,1,Item8);
    table->setItem(2,2,Item9);

    mainLay->addWidget(table);

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   //占满
    table->verticalHeader()->setVisible(false);   //隐藏右侧编号
    table->setSelectionBehavior(QAbstractItemView::SelectRows);   //点击时选中
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);    //无法编辑

    mainQ->show();

}

void test04()
{
    QWidget* mainQ = new QWidget;
    mainQ->setWindowTitle("列表控件");
    mainQ->resize(400, 600);
    mainQ->setFont(QFont("宋体", 14, QFont::Bold));
    QVBoxLayout* mainLay = new QVBoxLayout(mainQ);


    QListWidget* listwid = new QListWidget(mainQ);
    listwid->addItem("项目");

    QListWidgetItem* Item = new QListWidgetItem(QIcon(":/image/1.png"),"控件1");
    listwid->addItem(Item);

    QListWidgetItem* Item2 = new QListWidgetItem(QIcon(":/image/2.png"),"控件2");
    listwid->addItem(Item2);

    QListWidgetItem* Item3 = new QListWidgetItem(QIcon(":/image/3.png"),"控件3");   //QListWidget相当于书架 QListWidgetItem相当于书
    listwid->addItem(Item3);

    mainLay->addWidget(listwid);
    listwid->setViewMode(QListWidget::IconMode);
    listwid->setIconSize(QSize(100, 100));
    listwid->setSpacing(30);

    mainQ->show();

}

void test03()
{
    QWidget* mainQ = new QWidget;
    mainQ->setWindowTitle("下拉框");
    mainQ->resize(400, 600);
    mainQ->setFont(QFont("宋体", 14, QFont::Bold));
    QVBoxLayout* mainLay = new QVBoxLayout(mainQ);

    QComboBox* comb = new QComboBox(mainQ);   //变成main_w的子对象  当父对象 main_w（窗口）被关闭并销毁时，Qt 会自动遍历并销毁它的所有子对象

    comb->addItem("北京");   // 只要执行了 mainLay->addWidget(comb)，布局就会管理它的显示和生命周期
    comb->addItem("上海");
    comb->addItem("深圳");
    comb->setCurrentIndex(0);   //默认选中

    mainLay->addWidget(comb);


    mainQ->show();
}

void test02()
{
    QWidget* main_qwdget = new QWidget;
    main_qwdget->setWindowTitle("复选框按钮");
    main_qwdget->resize(600, 800);
    main_qwdget->setFont(QFont("宋体", 16, QFont::Black));

    QVBoxLayout* mainLay = new QVBoxLayout;

    QGroupBox* Qb = new QGroupBox("兴趣爱好", main_qwdget);
    QVBoxLayout* gLay = new QVBoxLayout(Qb);

    QCheckBox* tn1 = new QCheckBox("篮球", Qb);
    QCheckBox* tn2 = new QCheckBox("羽毛球", Qb);
    QCheckBox* tn3 = new QCheckBox("足球", Qb);    //创建一个样式的时候就要创建一个对应的分布方式来进行管理

    tn1->setChecked(true);

    gLay->addWidget(tn1);
    gLay->addWidget(tn2);
    gLay->addWidget(tn3);

    mainLay->addWidget(Qb);

    main_qwdget->show();
}

void test01()   //单选框
{
    QWidget* main_qwdget = new QWidget;
    main_qwdget->setWindowTitle("单选按钮");
    QVBoxLayout* V_layout = new QVBoxLayout;
    main_qwdget->setLayout(V_layout);
    main_qwdget->resize(600, 800);
    main_qwdget->setFont(QFont("宋体", 18, QFont::Black));

    QRadioButton* tn1 = new QRadioButton("男");
    QRadioButton* tn2 = new QRadioButton("女");
    QRadioButton* tn3 = new QRadioButton("其他");

    tn1->setChecked(true);   //默认tn1亮光标

    V_layout->addWidget(tn1);
    V_layout->addWidget(tn2);
    V_layout->addWidget(tn3);
    V_layout->addStretch();


    main_qwdget->show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setStyleSheet(R"(QPushButton{color:blue;})");   //应用级
    test01();
    test02();
    test03();
    test04();
    test05();
    test06();
    return QApplication::exec();
}
