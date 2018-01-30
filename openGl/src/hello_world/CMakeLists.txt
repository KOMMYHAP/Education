cmake_minimum_required(VERSION 3.8)
project(hello_world)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp)

add_executable(hello_world ${SOURCE_FILES})

target_link_libraries(hello_world glfw GL)