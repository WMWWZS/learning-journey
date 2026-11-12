#include "myfstream.h"
Myfstream::Myfstream()
{

}

Myfstream::Myfstream(const string fileName)
{
    this->open(fileName, ios::in | ios::out | ios::binary);
    if(is_open())
        cout << "[LOG]文件打开成功" << endl;
    else
        cerr << "文件打开失败" << endl;
}

long Myfstream::fileLength()
{
    if(is_open())  //只判断文件是否成功打开
    {
        cout << "[LOG]文件打开成功" << endl;
        seekg(0, ios::end);
        long size = tellg();
        seekg(0, ios::beg);
        return size;
    }
    else
    {
        cerr << "文件打开失败" << endl;
        return -1;
    }

}

bool Myfstream::writeToFile(const string s)
{
    if(*this)  //这个判断更加严格
    {
        seekg(0,ios::end);
        *this << s << endl;
        cout << "已经写入文本" << s << endl;
        return true;
    }
    else
    {
        cerr << "文件打开失败" << endl;
        return false;
    }
}

bool Myfstream::writeToFile(const void *data, int len)
{
    if(*this)
    {
        seekg(ios::end);
        write((char*)data, len);
        return true;
    }
    else
    {
        cerr << "文件打开失败" << endl;
        return false;
    }
}

std::string Myfstream::readLine()
{
    string line;
    if(*this)
    {
        if(std::getline(*this, line))   //两个行为从文件流读取一整行，存入 line；返回流对象（也就是 *this），可以隐式转布尔判断读取是否成功
        {
            cout << "当前行的文本：" << endl;
        }
    }
    else
    {
        cerr << "文件打开失败" << endl;
    }
}

std::string Myfstream::readAll()
{
    if(*this)
    {
        long size = this->fileLength();
        string buffer(size, '\0');
        read(&buffer[0], size);
        return buffer;
    }
    else
        return "";
}

bool Myfstream::readFromFile(void *data, int len)
{
    if(*this)
    {
        read((char*)data, len);
        cout << "当前读取的大小为：" << len << endl;
        return true;
    }
    return false;
}
