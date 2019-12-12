cmake_minimum_required(VERSION 3.15)
project(assignment4_hctob)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(assignment4_hctob main.cpp Array.hpp)

add_executable(test_array test_Array.cpp Array.hpp)
target_compile_options(test_array PRIVATE -Wall -Wextra -pedantic)