cmake_minimum_required(VERSION 3.16)
project(kvstore)

set(CMAKE_CXX_STANDARD 20)

# ======================================
# GoogleTest
# ======================================
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

# ======================================
# Library (core KV store)
# ======================================
add_library(kvstore_lib
    src/store.cpp
    src/sharded_store.cpp
)

target_include_directories(kvstore_lib PUBLIC include)

# ======================================
# Main executable
# ======================================
add_executable(kvstore_app
    src/main.cpp
)

target_link_libraries(kvstore_app kvstore_lib)

# ======================================
# Tests
# ======================================
add_executable(kvstore_test
    tests/store_test.cpp
)

target_link_libraries(
    kvstore_test
    kvstore_lib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(kvstore_test)
