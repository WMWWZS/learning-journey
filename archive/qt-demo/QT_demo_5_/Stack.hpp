#ifndef STACK_HPP
#define STACK_HPP
template <typename T>
class Stack
{
private:
    T* data;
    int maxSize;
    int cur;
public:
    Stack(int Size = 20)
    {
        maxSize = Size;
        cur = 0;
        data = new T[maxSize];
    }
    void push(T val)
    {
        if(cur < maxSize)
        {
            data[cur++] = val;
        }
    }
    void pop()
    {
        if(!empty())
        {
            cur--;
        }
    }

    T top()
    {
        return data[cur];
    }

    bool empty()
    {
        return cur == 0;
    }

    int size()
    {
        return cur;
    }

    ~Stack()
    {
        delete[] data;
    }

};


#endif // STACK_HPP
