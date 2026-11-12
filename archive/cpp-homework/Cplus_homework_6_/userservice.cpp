#include "userservice.h"
#include "cdata.h"
#include "user.h"


UserService::UserService() {}

bool UserService::registerUser(string name, string password)
{
    user u(name,password);
    Cdata::userV.push_back(u);
    Cdata::userM.insert(pair<string,user>(name,u));

    string fileName = "data.bat";
    fstream file;

    file.open(fileName, ios::app | ios::binary);
    USER_T newUser = {0};
    newUser.id = u.getUserId();
    strcpy(newUser.userName, u.getUserName().c_str());
    strcpy(newUser.passWord, u.getPassWord().c_str());    //返回值是string c_str() 转成 const char*
    newUser.state = u.getState();
    file.write((char*)(&newUser), sizeof(USER_T));    //文件参数要求 const* char
    file.close();
    return true;
}

bool UserService::loginByVector(string name, string password)
{
    for(vector<user>::iterator it = Cdata::userV.begin(); it != Cdata::userV.end(); it++)
    {
        if(it->getUserName() == name && it->getPassWord() == password) { return true; }
    }
    return false;
}

bool UserService::loginByMap(string name, string password)
{
    map<string,user>::iterator it = Cdata::userM.find(name);

    if(it == Cdata::userM.end()) { return false; }
    else
    {
        if(it->second.getPassWord() == password) { return true; }
    }
}

void UserService::addVideo(const string &name, const string &path)
{
    Cdata::videos.emplace_back(Cdata::nextVideoId, name, path);
    cout << "视频新增成功！" << endl;
    Cdata::nextVideoId++;
}

bool UserService::deleteVideoById(int id)
{
    for (auto it = Cdata::videos.begin(); it != Cdata::videos.end(); ++it)
    {
        if (it->getId() == id)
        {
            cout << "正在删除视频：";
            it->show();
            Cdata::videos.erase(it);
            cout << "删除成功！" << endl;
            return true;
        }
        cout << "未找到 ID 为 " << id << " 的视频。" << endl;
        return false;
    }
}

void UserService::findVideosByName(const string &keyword) const
{
    cout << "查询keyword：" << keyword << endl << "结果显示：" << endl;
    bool found = false;
    for(list<Video>::iterator it = Cdata::videos.begin(); it != Cdata::videos.end(); it++)
    {
        if(it->getName().find(keyword) != string::npos)
        {
            it->show();
            found = true;
        }
    }
    if(!found)
    {
        cout << "未找到" << keyword << endl;
    }
}

void UserService::showAllVideos() const
{
    cout << "所有视频列表" << endl;
    if (Cdata::videos.empty())
    {
        cout << "（暂无视频）" << endl;
        return;
    }
    for (const auto& v : Cdata::videos)
    {
        v.show();
    }
}