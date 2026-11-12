cmake_minimum_required(VERSION 3.16)

project(QT_demo_6_ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QT_demo_6_ main.cpp
    vector_demo.hpp
    list_demo.hpp
    map_demo.hpp
    deque_demo.hpp
    functor_demo.hpp
    algorithm_demo.hpp
)

include(GNUInstallDirs)
install(TARGETS QT_demo_6_
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
