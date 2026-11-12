#ifndef USERSERVICE_H
#define USERSERVICE_H
#include <iostream>

using namespace std;


class UserService
{
public:
    UserService();
    bool registerUser(string name, string password);
    bool loginByVector(string name, string password);
    bool loginByMap(string name, string password);

    void addVideo(const std::string& name, const std::string& path);
    bool deleteVideoById(int id);
    void findVideosByName(const std::string& keyword) const;
    void extracted() const;
    void showAllVideos() const;
};

#endif // USERSERVICE_H
