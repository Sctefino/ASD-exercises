cmake_minimum_required(VERSION 3.21)
project(Es1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Es1 main.c PQ.h PQ.c Item.c Item.h diag.c diag.h)
