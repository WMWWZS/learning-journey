cmake_minimum_required(VERSION 3.16)

project(Cplus_homework_6_ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cplus_homework_6_ main.cpp
    user.h user.cpp
    cdata.h cdata.cpp
    userservice.h userservice.cpp
    video.h video.cpp
)

include(GNUInstallDirs)
install(TARGETS Cplus_homework_6_
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
