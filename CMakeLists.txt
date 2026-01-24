cmake_minimum_required(VERSION 3.16)
project(kvstore)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(kvstore ${SRC_FILES})
