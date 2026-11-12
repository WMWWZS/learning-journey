#ifndef VIDEO_H
#define VIDEO_H

#include <string>

class Video {
private:
    int id;
    std::string name;
    std::string path;
    std::string createTime;

    std::string getCurrentTime();

public:
    Video(int vid, const std::string& vname, const std::string& vpath);

    int getId() const;
    std::string getName() const;
    std::string getPath() const;
    std::string getCreateTime() const;

    void show() const;
};

#endif