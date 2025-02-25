cmake_minimum_required(VERSION 3.21)
project(Es1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Es1 gdr.c equipArray.h inv.h invArray.h pg.h pgList.h inv.c invArray.c pg.c pgList.c equipArray.c)
