cmake_minimum_required(VERSION 3.20)
project(Metro)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
add_library(Metro_lib metro.cpp)
target_include_directories(Metro_lib PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(Metro main.cpp)

target_link_libraries(Metro PRIVATE Metro_lib)
