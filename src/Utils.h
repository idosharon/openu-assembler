#ifndef UTILS_HEADER
#define UTILS_HEADER

#include "Includes.h"
#include "Consts.h"
#include "Types.h"

#define is_equal(a, b) (strcmp(a, b) == 0)

#define info(msg) printf("[info]: %s\n", msg)
#define info_file(msg, file_name) printf("[info]: %s: %s\n", msg, file_name)

int appendBinaryToFile(FILE*, word);
word calculateOnsComplete(word*);

bool is_number(char*);
int find_command(char*);
int get_command_length(int);

#endif
