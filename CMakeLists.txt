cmake_minimum_required(VERSION 3.22)
project(Memory_Allocator)

set(CMAKE_CXX_STANDARD 14)

add_executable(Memory_Allocator
        main.cc Screen.cc Screen.h Airplane.cc Airplane.h)
