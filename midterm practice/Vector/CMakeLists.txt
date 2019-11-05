cmake_minimum_required(VERSION 3.15)
project(Vector)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Vector
        main.cpp
        makefile
        Vector.hpp)
