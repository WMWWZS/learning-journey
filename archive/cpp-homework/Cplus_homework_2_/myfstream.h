#ifndef MYFSTREAM_H
#define MYFSTREAM_H
#include <fstream>
#include <iostream>
using namespace std;
class Myfstream : public fstream
{
public:
    Myfstream();
    Myfstream(const string fileName);
    long fileLength();
    bool writeToFile(const string s);
    bool writeToFile(const void* data, int len);
    string readLine();
    string readAll();
    bool readFromFile(void* data, int len);
};

#endif // MYFSTREAM_H
