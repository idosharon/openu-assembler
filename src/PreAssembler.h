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

char* preAssemble(FILE*, char*);
macro_t* findMacro(char*, macro_node_t*);

bool isValidMacroName(char*);
macro_node_t* addMacroNode(macro_node_t*, char*);


#endif
