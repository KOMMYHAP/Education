cmake_minimum_required(VERSION 3.9)
project(parallel_accumulating)

set(CMAKE_CXX_STANDARD 11)

add_executable(parallel_accumulating
        main.cpp)

target_link_libraries(parallel_accumulating pthread)