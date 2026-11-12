#include "cdata.h"
//初始化容器
vector<user> Cdata::userV;
map<string,user> Cdata::userM;
list<Video> Cdata::videos;

int Cdata::nextVideoId = 1;

void Cdata::initUsers()
{
    string fileName = "data.txt";
    fstream file;
    file.open(fileName, ios::in | ios::binary);
    if (file.is_open())
    {
        cout << "文件已存在，直接打开" << endl;
        return;
    }
    else
    {   //会自动创建
        file.open(fileName, ios::out | ios::binary);
        if(!file.is_open())
        {
            cout << "文件打开失败" << endl;
            return;
        }
        //写入一个初始用户数据到文件中
        USER_T u = {user::getSeq(), "admin", "123456"};
        file.write((char*)(&u), sizeof(USER_T));
        file.close();
        file.open(fileName , ios::in | ios::binary);
    }
    while(true)
    {
        USER_T  u2 = {0};
        file.read((char*)(&u2),sizeof(USER_T));
        if(file.eof())
        {
            cout << "文件已读取完成" << endl;
            break;
        }
        user newUser(string(u2.userName), string(u2.passWord));
        Cdata::userV.push_back(newUser);
        Cdata::userM.insert(make_pair(u2.userName, newUser));

        for(map<string,user>::iterator it = userM.begin(); it != userM.end(); it++)
        {
            cout << it->first << "  " << it->second.getPassWord() << endl;
        }
    }
}


Cdata::Cdata() {}
