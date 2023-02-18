#ifndef PRE_ASSEMBLER_HEADER
#define PRE_ASSEMBLER_HEADER

#include "Consts.h"
#include "Includes.h"

#define PRE_ASSEMBLER_FILE_TYPE ASM_FILE_TYPE

typedef struct {
    char* name;
    char* data;
} macro_t;

typedef struct {
    macro_t* macro;
    struct macro_list_t* next;
} macro_list_t;

void pre_assemble(FILE* file, char* file_name);





#endif
