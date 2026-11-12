cmake_minimum_required(VERSION 3.16)

project(Cplus_homework_1_ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cplus_homework_1_ main.cpp
    Cplus_homework_1_.cpp
    Cplus_homework_1_.h)

include(GNUInstallDirs)
install(TARGETS Cplus_homework_1_
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
