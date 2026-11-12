#include "vector_demo.hpp"
#include "list_demo.hpp"
#include "deque_demo.hpp"
#include "map_demo.hpp"
#include "functor_demo.hpp"
#include "algorithm_demo.hpp"
using namespace std;



void test01()
{
    //1.vector的创建和添加元素
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);

    cout << "size = " << v.size() << endl;
    cout << "capacity = " << v.capacity() << endl;

    //2.访问元素
    cout << "v[1] = " << v[1] << endl;
    cout << "v.at(1) = " << v.at(1) << endl;
    cout << "v.front = " << v.front() << endl;
    cout << "v.back = " << v.back() << endl;

    //3.遍历方式
    cout << "下标遍历:" << endl;
    for(int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;

    cout << "迭代器遍历：" << endl;  //本质指针
    for(vector<int>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;


    cout << "for循环（c11）" << endl;
    for(int x : v)
    {
        cout << x << " ";
    }
    cout << endl;

    //4.插入及其删除
    v.insert(v.begin() + 1, 99);//位置偏移，在下标1的位置插入99
    cout << "插入后：" << endl;
    for(int x : v)
    {
        cout << x << " ";
    }
    cout << endl;

    v.erase(v.begin()); //删除第一个元素
    cout << "删除后：" << endl;
    for(int x : v)
    {
        cout << x << " ";
    }
    cout << endl;

    v.pop_back();//删除最后一个元素
    cout << "删除后：" << endl;
    for(int x : v)
    {
        cout << x << " ";
    }
    cout << endl;

    //5.存储自定义类型
    vector<fruit> fruits;
    fruits.push_back(fruit("苹果", 3.5));
    fruits.push_back(fruit("无花果", 7.5));
    fruits.push_back(fruit("西瓜", 4.5));
    fruits.push_back(fruit("香蕉", 6.5));

    for(vector<fruit>::iterator it = fruits.begin(); it != fruits.end(); it ++)  //it可以随便命名
    {
        it->show();
    }

    //6.清空
    fruits.clear();
    cout << "清空fruits后：" << endl;
    cout << "size = " << fruits.size() << endl;
    cout << "capacity = " << fruits.capacity() << endl;
}





void test02()
{
    int choice;
    StudentQueue sq;
    string name;

    do
    {
        cout << "1. 学生加入队伍（队尾）" << endl;
        cout << "2. 学生插队（队首）" << endl;
        cout << "3. 学生打饭离开（队首）" << endl;
        cout << "4. 查看当前队列" << endl;
        cout << "5. 查找学生是否在队伍中" << endl;
        cout << "0. 退出" << endl;
        cout << "请选择操作: ";
        cin >> choice;

        switch(choice)
        {
        case 1:
            cout << "请输入加入队伍的学生姓名: ";
            cin >> name;
            sq.joinQueue(name);
            break;
        case 2:
            cout << "请输入插队的学生姓名: ";
            cin >> name;
            sq.cutlineQueue(name);
            break;
        case 3:
            sq.leaveQueue();
            break;
        case 4:
            sq.showQueue();
            break;
        case 5:
            cout << "请输入要查找的学生姓名: ";
            cin >> name;
            sq.findStudent(name);
            break;
        case 0:
            cout << "退出系统" << endl;
            break;
        default:
            cout << "无效选择" << endl;
        }
    }while(choice != 0);
}





void test03()
{
    deque<int> dq;

    //1.数据插入
    dq.push_back(1);
    dq.push_back(2);
    dq.push_front(3);
    dq.push_front(4);

    for(int x : dq)
    {
        cout << x << " ";
    }
    cout << endl;

    for(int i = 0; i < dq.size(); i++)  //下标访问
    {
        cout << dq[i] << " ";
    }
    cout << endl;

    dq.pop_back();   //删尾
    dq.pop_front();  //删头
    cout << "删除两端后: ";
    for (int x : dq)
        cout << x << " ";  // 5 10
    cout << endl;
}





void test04()
{
    //1.创建和插入  //first-second —— 键?值
    map<string, int> scores;
    scores.insert(pair<string, int>("张三", 99));  //手动写数据类型
    scores.insert(make_pair("张三", 99));    //自动推导数据类型
    scores["王五"] = 79;  //存在王五，将值改成79，若不存在，则自动创建

    //2.遍历
    for(map<string ,int>::iterator it = scores.begin(); it != scores.end(); it++)
    {
        cout << it->first << ": " << it->second << endl;
    }

    //3.查找
    string name = "王五";
    map<string, int>::iterator it = scores.find(name);
    if (it != scores.end())  //走不到尾指针就是找到了
    {
        cout << name << "的成绩是: " << it->second << "分" << endl;
    }
    else
    {
        cout << name << " 不存在" << endl;
    }

    scores.erase("王五");
    cout << "删除后" << endl;
    for(map<string ,int>::iterator it = scores.begin(); it != scores.end(); it++)
    {
        cout << it->first << ": " << it->second << endl;
    }
}





void test05()
{
    //1.自定义函数对象
    Adder add;
    cout << add(2, 6) << endl;  //可以反复干活的add对象
    cout << Adder()(10, 3) << endl;   //用完就摧毁，一次性

    vector<int> nums = {7, 3, 1, 9, 10, 4};

    cout << "排序前: ";
    for (int x : nums) cout << x << " ";
    cout << endl;

    sort(nums.begin(), nums.end(), Adder2());  //sort默认是升序，但是Adder2可以改变sort2的规则

    cout << "排序后: ";
    for (int x : nums) cout << x << " ";
    cout << endl;  //降序了

    //2.STL内建函数对象
    vector<int> nums2 = {3, 5, 1, 7, 0};
    sort(nums2.begin(), nums2.end(), less<int>());  //给模板传入类型参数，实例化得到具体类 less<int>，()就是调用一个临时对象来用
    cout << "排序后: ";
    for (int x : nums2) cout << x << " ";
    cout << endl;

    sort(nums2.begin(), nums2.end(), greater<int>());
    cout << "排序后: ";
    for (int x : nums2) cout << x << " ";
    cout << endl;
}





void test06()
{
    cout << "常见的STL算法：" << endl;
    vector<int>n = {5, 1, 3, 6, 8, 2, 4, 5 ,4 ,1, 4 ,5 };
    vector<int>n2 = {1, 3, 4, 5, 7, 8, 9};

    //1.排序
    sort(n.begin(), n.end());
    printfVet(n);


    //2.查找
    auto it = find(n.begin(), n.end(), 2);
    if(it != n.end())
    {
        cout << "找到了: " << *it << endl;
    }

    //3.二分查找(要求有序）
    bool found = binary_search(n2.begin(), n2.end(), 1);
    cout << (found ? "已找到" : "未找到") << "1" << endl;

    //4.计数
    int cnt = count(n.begin(), n.end(), 1);
    cout << "1的出现次数：" << cnt << endl;

    //5.求和
    int sum = accumulate(n.begin(), n.end(), 0);
    cout << "总和: " << sum << endl;


    //6.求极值
    auto min = min_element(n.begin(), n.end());   //放回iterator迭代器
    auto max = max_element(n.begin(), n.end());
    cout << "最小值: " << *min << ", 最大值: " << *max << endl;

}





int main()
{
    // test01();
    // test02();
    // test03();
    // test04();
    // test05();
    test06();
    return 0;
}
