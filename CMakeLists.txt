# CMakeList for the OpenU assembler project
# Course: 20465 - System Programming
# Lecturer: Ram Tahor
# Author: Ido Sharon 215774142
#		  Amitai Ben Shalom 327743399
# Date: 2023-02-17

cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME assembler)
project(${PROJECT_NAME} C)
# Set the C standard to C90
set(CMAKE_C_STANDARD 90)

# set flags for the compiler
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -ansi -pedantic -std=c90")

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set the output directory for the object files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)

# Set source directory
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# get all the source files
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.c)
file(GLOB_RECURSE HEADER_FILES ${SOURCE_DIR}/*.h)

# compile the source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} src/Consts.c)
