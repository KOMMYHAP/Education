cmake_minimum_required(VERSION 3.9)
project(concurrency)

set(CMAKE_CXX_STANDARD 11)

add_executable(concurrency main.cpp)

target_link_libraries(concurrency pthread)