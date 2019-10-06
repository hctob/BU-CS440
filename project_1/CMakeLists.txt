cmake_minimum_required(VERSION 3.14)
project(container_in_c_hctob)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++17 -Wl,--no-as-needed -ld" )

include_directories(.)

add_executable(container_in_c_hctob
        test.cpp
        Deque.hpp
        makefile
        README)
