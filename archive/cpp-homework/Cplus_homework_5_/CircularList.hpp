#pragma once  //防止重复编译

#include <iostream>
#include <cstddef>

using namespace std;

template <class T>
class CircularList
{
private:
    struct Node
    {
        Node* prev;
        Node* next;
        T data;
        Node(const T& val, Node* p = nullptr, Node* n = nullptr) : data(val), prev(p), next(n) {}
    };

    Node* head = nullptr;
    size_t size = 0;

public:
    CircularList() = default;

    size_t getSize() const
    {
        return size;
    }

    bool empty() const
    {
        return head == nullptr;
    }

    void pushFront(const T& val)  //头插
    {
        Node* newNode = new Node(val);
        if(empty())
        {
            newNode->next = newNode;
            newNode->prev = newNode;
            head = newNode;
        }
        else
        {
            Node* tail = head->prev;

            newNode->next = head;
            newNode->prev = tail;

            head->prev = newNode;   //这里写成head-> 导致错误
            tail->next = newNode;

            head = newNode;
        }
        size++;
    }

    void pushBack(const T& val)  //尾插
    {
        Node* newNode = new Node(val);
        if(empty())
        {
            newNode->next = newNode;
            newNode->prev = newNode;
            head = newNode;
        }
        else
        {
            Node* tail = head->prev;
            newNode->next = head;
            newNode->prev = tail;

            //旧尾巴处理
            tail->next = newNode;
            head->prev = newNode;

        }
        size++;
    }
    //头删：删除链表最开头的 head 结点，释放内存，闭环重新衔接
    void popFront(T& val) //val 是外面传进来变量的别名，可以把被删掉头部节点的数据带出去给调用者
    {
        if (empty())
        {
            return;
        }
        Node* oldHead = head;
        val = oldHead->data;
        if(size == 1)
        {
            //只剩一个节点
            head = nullptr;
        }
        else
        {
            Node* tail = head->prev;
            Node* newHead = head->next;

            newHead->prev = tail;
            tail->next = newHead;

            head = newHead;  //更新
        }

        delete oldHead;
        size--;
    }

    void popBack(T& val)   //尾删
    {
        if(empty())
        {
            return;
        }
        if (size == 1)  //仅剩一个节点
        {
            popFront(val);
            return;
        }
        Node* tail = head->prev;
        val = tail->data;

        Node* newTail = tail->prev;
        newTail->next = head;
        head->prev = newTail;
        delete tail;  //释放旧尾巴结点
        --size;
    }

    void debugPrint()
    {
        if (empty())
        {
            cout << "(empty)" << endl;
            return;
        }

        cout << "[head] ";
        Node* cur = head;
        do
        {
            cout << cur->data << " <-> ";
            cur = cur->next;
        } while (cur != head);
        cout << "[head]" << endl;
    }
};