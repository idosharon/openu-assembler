# Makefile for the OpenU assembler project
# Course: 20465 - System Programming
# Lecturer: Ram Tahor
# Author: Ido Sharon 215774142
#		  Amitai Ben Shalom 327743399
# Date: 2023-02-17

# Set the compiler and flags
CC = gcc
C_VERSION = c90
C_FLAGS = -Wall -ansi -pedantic -std=$(C_VERSION)

# project name
PROJECT_NAME = openu-assembler

# folders
SRC_FOLDER = src
OBJ_FOLDER = obj
BIN_FOLDER = bin

# object files
SOURCE_FILES = $(wildcard $(SRC_FOLDER)/*.c)

# the executable file
EXECUTABLE = $(BIN_FOLDER)/$(PROJECT_NAME)

# run the project
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# compile and run
compile: clean $(EXECUTABLE)
	./$(EXECUTABLE)

# create the executable file, all object files must be created first
$(EXECUTABLE): $(SOURCE_FILES:$(SRC_FOLDER)/%.c=$(OBJ_FOLDER)/%.o)
	$(CC) $(C_FLAGS) $(OBJECTS) -o $@

# for each source file, create an object file
$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.c
	$(CC) $(C_FLAGS) -c $< -o $@

# clean the project
clean:
	rm -f $(OBJ_FOLDER)/*.o
	rm -f $(BIN_FOLDER)/$(PROJECT_NAME)
