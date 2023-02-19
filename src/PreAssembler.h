#ifndef PRE_ASSEMBLER_HEADER
#define PRE_ASSEMBLER_HEADER

#include "Types.h"
#include "Includes.h"

#define PRE_ASSEMBLER_FILE_EXTENSION "_"ASM_FILE_EXTENSION

#define START_MACRO_SYMBOL "mcr"
#define END_MACRO_SYMBOL "endmcr"


typedef struct {
    char* name;
    char* data;
} macro_t;

typedef struct {
    macro_t* macro;
    struct macro_node_t* next;
} macro_node_t;

void pre_assemble(FILE*, char*);
macro_t* find_macro(char*, macro_node_t*);

bool is_valid_macro_name(char*);


#endif
