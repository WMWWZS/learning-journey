#include "video.h"
#include <time.h>
#include <iostream>

std::string Video::getCurrentTime()
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
            1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return std::string(buffer);
}

Video::Video(int vid, const std::string &vname, const std::string &vpath) : id(vid), name(vname),path(vpath)
{
    createTime = getCurrentTime();
}

int Video::getId() const { return id; }
std::string Video::getName() const { return name; }
std::string Video::getPath() const { return path; }
std::string Video::getCreateTime() const { return createTime; }



void Video::show() const
{
    std::cout << "ID: " << id<< ", 名称: " << name << ", 路径: " << path<< ", 创建时间: " << createTime << std::endl;
}
