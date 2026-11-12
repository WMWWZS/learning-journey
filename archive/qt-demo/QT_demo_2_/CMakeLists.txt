cmake_minimum_required(VERSION 3.16)

project(QT_demo_2_ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QT_demo_2_ main.cpp
    phone.h phone.cpp
    employee.h employee.cpp)

include(GNUInstallDirs)
install(TARGETS QT_demo_2_
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
