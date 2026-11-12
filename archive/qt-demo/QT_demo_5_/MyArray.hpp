#ifndef MYARRAY_HPP
#define MYARRAY_HPP

template <typename T>
class MyArray
{
private:
    T* arr;
    int capacity;
public:
    MyArray(int cap)
    {
        arr = new T[cap];
    }

    T& operator[](int index)
    {
        return arr[index];
    }

    int getLen()
    {
        return capacity;
    }

    ~MyArray()
    {
        delete[] arr;
    }
};


#endif // MYARRAY_HPP
