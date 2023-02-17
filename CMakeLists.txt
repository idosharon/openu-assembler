cmake_minimum_required(VERSION 3.24)
project(assembler_project C)

set(CMAKE_C_STANDARD 11)
# set c version to c90
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -ansi -pedantic -std=c90")
# add warning flags


add_executable(assembler_project
        main.c)
