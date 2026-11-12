#include "cdata.h"
#include "userservice.h"

using namespace std;

void test07()
{
    Cdata::initUsers();
}

void test08()
{
    int choose;
    Cdata::initUsers();
    UserService Us;
    string name;
    string paw;
    while(true)
    {
        cout << "1.注册" << endl;
        cout << "2.基于Vector的登录" << endl;
        cout << "3.基于Map的登录" << endl;
        cout << "0.退出" << endl;
        cout << "请选择" << endl;
        cin >> choose;
        switch(choose)
        {
            case 1:
            {
                cout << "请输入用户名：" << endl;
                cin >> name;
                cout << "请输入密码：" << endl;
                cin >> paw;
                bool flag = Us.registerUser(name, paw);
                if(flag)
                {
                    cout << "注册成功" << endl;
                }
                else
                {
                    cout << "注册失败" << endl;
                }
                break;
            }
            case 2:
            {
                cout << "请输入用户名：" << endl;
                cin >> name;
                cout << "请输入密码：" << endl;
                cin >> paw;
                bool flag = Us.loginByVector(name, paw);
                if(flag)
                {
                    cout << "登录成功" << endl;
                }
                else
                {
                    cout << "登录失败" << endl;
                }
                break;
            }
            case 3:
            {
                cout << "请输入用户名：" << endl;
                cin >> name;
                cout << "请输入密码：" << endl;
                cin >> paw;
                bool flag = Us.loginByMap(name, paw);
                if(flag)
                {
                    cout << "登录成功" << endl;
                }
                else
                {
                    cout << "登录失败" << endl;
                }
                break;
            }
            case 0:
            {
                break;
            }
            default:
            {
                cout << "无效输入" << endl;
                break;
            }
        }
    }

}

void test09()
{
    int op;
    string name, path;
    UserService Us;
    int id;

    while (true)
    {
        cout << "\n--- 视频管理菜单 ---" << endl;
        cout << "1. 新增视频" << endl;
        cout << "2. 删除视频（按ID）" << endl;
        cout << "3. 查询视频（按名称关键字）" << endl;
        cout << "4. 显示全部视频" << endl;
        cout << "5. 退出视频管理" << endl;
        cout << "请选择: ";
        cin >> op;

        switch (op)
        {
        case 1:
            cout << "请输入视频名: ";
            cin >> name;
            cout << "请输入视频路径: ";
            cin >> path;
            Us.addVideo(name, path);
            break;
        case 2:
            cout << "请输入要删除的视频ID: ";
            cin >> id;
            Us.deleteVideoById(id);
            break;
        case 3:
            cout << "请输入查询关键字: ";
            cin >> name;
            Us.findVideosByName(name);
            break;
        case 4:
            Us.showAllVideos();
            break;
        case 5:
            cout << "退出视频管理，返回主菜单。" << endl;
            return;
        default:
            cout << "无效选择，请重新输入。" << endl;
        }
    }
}

int main()
{
    // test07();
    // test08();
    test09();
    return 0;
}
