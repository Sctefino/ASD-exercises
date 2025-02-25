cmake_minimum_required(VERSION 3.21)
project(Es3 C)

set(CMAKE_C_STANDARD 99)

add_executable(Es3 main.c data.h ora.h titolo.h coll_titoli.h quotazione.h coll_quotazioni.h titolo.c coll_titoli.c coll_quotazioni.c data.c ora.c quotazione.c)
