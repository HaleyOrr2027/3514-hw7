cmake_minimum_required(VERSION 3.20)
project(A7)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(tests)

add_executable(tests
    tests/test.cpp
)