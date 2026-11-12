cmake_minimum_required(VERSION 3.16)

project(Cplus_homework_3_ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cplus_homework_3_ main.cpp
    myfstream.cpp myfstream.h user.cpp user.h usermodel.cpp usermodel.h)

include(GNUInstallDirs)
install(TARGETS Cplus_homework_3_
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
