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

FILES = main
OBJECTS = $(addprefix $(OBJ_FOLDER)/, $(addsuffix .o, $(FILES)))

EXECUTABLE = $(BIN_FOLDER)/$(PROJECT_NAME)

# check if the obj file of each source file exists
main: $(OBJECTS)
	# link all .o files into the executable
	$(CC) $(C_FLAGS) $^ -o $(EXECUTABLE)
	./$(EXECUTABLE)

# for each file in FILES create a rule to compile it
$(addprefix $(OBJ_FOLDER)/, $(addsuffix .o, $(FILES))):
	# compile each .c file into .o file
	$(CC) $(C_FLAGS) -c $(SRC_FOLDER)/$(basename $(notdir $@)).c -o $@

clean:
	rm -f $(OBJ_FOLDER)/*.o $(EXECUTABLE)
