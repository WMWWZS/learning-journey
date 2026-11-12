#ifndef CDATA_H
#define CDATA_H
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string.h>
#include <list>
#include "video.h"
#include "user.h"

using namespace std;

typedef struct
{
    int id;
    char userName[16];
    char passWord[16];
    int state;
}USER_T;

class Cdata
{
public:
    Cdata();
    static vector<user> userV;
    static map<string,user> userM;
    static list<Video> videos;

    static int nextVideoId;
    static void initUsers();

};

#endif // CDATA_H
