#ifndef UTILS_HEADER
#define UTILS_HEADER

#include "Includes.h"
#include "Consts.h"
#include "Types.h"

#define isStrEqual(a, b) (strcmp(a, b) == 0)

#define info(msg) printf("[info]: %s\n", msg)
#define info_file(msg, file_name) printf("[info]: %s: %s\n", msg, file_name)

int appendBinaryToFile(FILE*, word);
word calculateOnsComplete(word*);

bool is_number(char*);
int find_command(char*);
int get_command_length(char*);
int find_register(char*);
char* getRegisterNumber(int);
char* getOpcode(int);
char* reverse_string(char*);

char* getFileName(char*, char*);
FILE* openFile(char*, char*);

void free_list(node_t*);

#endif
