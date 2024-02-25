cmake_minimum_required(VERSION 3.23)
project(test_clion_nested C)

set(CMAKE_C_STANDARD 99)
set(GCC_COVERAGE_COMPILE_FLAGS "-fno-omit-frame-pointer -g -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(test_clion main.c)
