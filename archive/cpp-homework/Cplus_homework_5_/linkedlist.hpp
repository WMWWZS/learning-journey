#pragma once

#include <iostream>
#include <cstddef>
using namespace std;

template <class T>
class Linkedlist
{
private:
    struct Node
    {
        T data;
        Node* next;

        Node(const T& v, Node* n = nullptr) : data(v), next(n) {}
    };

    Node* head = nullptr;  //头指针，管理整一条
    size_t size = 0;   //节点数

public:
    Linkedlist() = default;

    ~Linkedlist()
    {
        while(!empty())  //循环全部释放
        {
            T temp;
            popFront(temp);
        }
    }

    bool empty()
    {
        return head == nullptr;
    }

    void popFront(T& v)  //这里加了const，试图给常量v赋值，导致直接错误，遂去掉
    {
        if(!empty())
        {
            return;
        }
        Node* oldNode = head;
        v = head->data;
        head = head->next;
        delete oldNode;
        size--;
    }

    void pushFront(const T& v)  //头插
    {
        head = new Node(v, head);  //Node的构造已经连接链表
        size++;
    }

    size_t getSize() const {
        return size;
    }

    void debugPrint()
    {
        Node* cur = head;
        while (cur != nullptr)
        {
            cout << cur->data << " -> ";
            cur = cur->next;
        }
        cout << "null" << endl;
    }

    T get(int index)
    {
        if(index < 0 || index >= size)  //假设 0 1 2
        {
            cout << "该索引越界" << endl;
        }
        Node* cur = head;
        for(int i = 0; i < index; i++)
        {
            cur = cur->next;
        }
        return cur->data;
    }

    int remove(int index)
    {
        if(index < 0 || index >= size)  //假设 0 1 2
        {
            cout << "该索引越界" << endl;
        }

        T val;
        if(index == 0) //头节点
        {
            Node* n = head;
            val = head->data;
            head = head->next;
            delete head;
        }
        else
        {
            Node* cur = head;
            for(int i = 0; i < index - 1; i++)  //找到前驱节点
            {
                cur = cur->next;
            }
            Node* del = cur->next;
            val = del->data;
            cur->next = del->next;
            delete del;
        }
        size--;
        return val;
    }
};
