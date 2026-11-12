#pragma once
#include <iostream>
#include <list>
#include <algorithm>
#include <string>

using namespace std;

class StudentQueue
{
private:
    list<string> queue;
public:
    //1.学生加入
    void joinQueue(const string& name)
    {
        queue.push_back(name);
        cout << name << "学生已加入队伍" << endl;
    }

    //2.插队
    void cutlineQueue(const string& name)
    {
        queue.push_front(name);
        cout << name << "学生已插入队伍前面" << endl;
    }

    //3.离队
    void leaveQueue()
    {
        if(queue.empty())
        {
            cout << "没人排队" << endl;
            return;
        }
        cout << queue.front() << "学生已打饭离开" << endl;
        queue.pop_front();
    }

    //4.显示队列信息
    void showQueue() const  //承诺这个函数不会修改类里面任何成员变量，只读函数
    {
        if(queue.empty())
        {
            cout << "没人排队" << endl;
            return;
        }

        int idx = 1;
        for(list<string>::const_iterator it = queue.begin(); it != queue.end(); it++)
        {
            cout << idx << " " << *it << endl;
            idx++;
        }
        cout << endl;
    }

    //5.找到指定学生
    void findStudent(const string& name) const
    {
        list<string>::const_iterator it = find(queue.begin(), queue.end(), name);
        if(it != queue.end())
        {
            cout << name << "已在队伍中" << endl;
        }
        else
        {
            cout << name << "未在队伍中" << endl;
        }
    }

};

