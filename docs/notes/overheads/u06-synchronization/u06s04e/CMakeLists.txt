cmake_minimum_required(VERSION 3.21)
project(u09s01e)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(basics threads-basics.cpp)
add_executable(print-prime print-prime.cpp)
add_executable(print-prime-synced print-prime-synced.cpp)
add_executable(taskQueue taskQueue.cpp)
