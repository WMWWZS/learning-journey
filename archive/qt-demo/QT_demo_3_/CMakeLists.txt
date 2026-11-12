cmake_minimum_required(VERSION 3.16)

project(QT_demo_3_ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QT_demo_3_ main.cpp
    warrior.h warrior.cpp
    mage.h mage.cpp
    archer.h archer.cpp
    character.h character.cpp
    father.h father.cpp
    son1.h son1.cpp
    son2.h son2.cpp
    son3.h son3.cpp
    myfstream.h myfstream.cpp)

include(GNUInstallDirs)
install(TARGETS QT_demo_3_
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
