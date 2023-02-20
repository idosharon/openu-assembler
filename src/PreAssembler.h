#ifndef PRE_ASSEMBLER_HEADER
#define PRE_ASSEMBLER_HEADER

#include "Types.h"
#include "Includes.h"

typedef struct {
    char* name;
    char* data;
} macro_t;

typedef struct {
    macro_t* macro;
    struct macro_node_t* next;
} macro_node_t;

char* pre_assemble(FILE*, char*);
macro_t* find_macro(char*, macro_node_t*);

bool is_valid_macro_name(char*);


#endif
