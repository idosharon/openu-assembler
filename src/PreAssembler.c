#include "PreAssembler.h"

void pre_assemble(FILE* file, char* file_name) {
    char line[MAX_LINE_SIZE];
    char* token;

    macro_list_t macroList;

    while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
        if ()
    }
}

macro_t* find_macro(char* token) {

}
