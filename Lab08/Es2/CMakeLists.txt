cmake_minimum_required(VERSION 3.21)
project(Es2 C)

set(CMAKE_C_STANDARD 99)

add_executable(Es2 main.c GRAPH.h GRAPH.c ST.c ST.h)
