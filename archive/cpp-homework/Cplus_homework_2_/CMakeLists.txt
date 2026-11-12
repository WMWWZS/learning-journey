cmake_minimum_required(VERSION 3.16)

project(Cplus_homework_2_ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE OFF)
endif()

add_executable(Cplus_homework_2_
    main.cpp
    user.h user.cpp
    usermodel.h usermodel.cpp
    myfstream.h
    myfstream.cpp
)
include(GNUInstallDirs)
install(TARGETS Cplus_homework_2_
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}


)
