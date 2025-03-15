cmake_minimum_required(VERSION 3.29)
project(Metro)

set(CMAKE_CXX_STANDARD 20)

add_executable(Metro main.cpp
        mtr.cpp
        mtr.h)
