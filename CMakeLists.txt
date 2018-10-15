cmake_minimum_required(VERSION 3.10)
project(escalona_refact C)

set(CMAKE_C_STANDARD 99)

add_executable(escalona_refact main.c main.h dataStructures.h dataStructures.c logs.c logs.h)