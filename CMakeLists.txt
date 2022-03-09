cmake_minimum_required(VERSION 3.21)
project(pptsm)

set(CMAKE_CXX_STANDARD 14)

include_directories(headers)

add_executable(assembler sources/assembler.cpp)

add_executable(cpu sources/cpu.cpp)
