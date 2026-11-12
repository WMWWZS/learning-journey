cmake_minimum_required(VERSION 3.16)

project(QT_demo_1_ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QT_demo_1_ main.cpp)

include(GNUInstallDirs)
install(TARGETS QT_demo_1_
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
