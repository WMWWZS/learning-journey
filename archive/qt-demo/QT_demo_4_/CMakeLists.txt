cmake_minimum_required(VERSION 3.16)

project(QT_demo_4_ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QT_demo_4_ main.cpp
    animal.h animal.cpp
    dog.h dog.cpp
    cat.h cat.cpp
    lion.h lion.cpp
    vehicle.h vehicle.cpp
    bicycle.h bicycle.cpp
    car.h car.cpp
    plane.h plane.cpp
    basetask.h basetask.cpp
    logintask.h logintask.cpp
    regtask.h regtask.cpp
    user.h user.cpp
    cdata.h cdata.cpp
    ../../C++_Homework/Cplus_homework_3_/Cplus_homework_3_/myfstream.cpp ../../C++_Homework/Cplus_homework_3_/Cplus_homework_3_/myfstream.h ../../C++_Homework/Cplus_homework_3_/Cplus_homework_3_/usermodel.cpp ../../C++_Homework/Cplus_homework_3_/Cplus_homework_3_/usermodel.h)

include(GNUInstallDirs)
install(TARGETS QT_demo_4_
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
