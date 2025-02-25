cmake_minimum_required(VERSION 3.21)
project(Es3 C)

set(CMAKE_C_STANDARD 99)

add_executable(Es3 client.c personaggi.h personaggi.c inventario.c inventario.h)
