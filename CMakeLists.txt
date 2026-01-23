cmake_minimum_required(VERSION 3.16)
project(kvstore)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(kvstore ${SRC_FILES})
