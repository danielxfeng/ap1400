cmake_minimum_required(VERSION 3.18)
project(src)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(src
        algebra.cpp
        main.cpp
        unit_test.cpp
        algebra.h)
