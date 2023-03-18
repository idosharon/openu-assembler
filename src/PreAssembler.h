#ifndef PRE_ASSEMBLER_HEADER
#define PRE_ASSEMBLER_HEADER

#include "Types.h"
#include "Includes.h"

typedef struct {
    char* name;
    char* data;
    size_t lines_count;
} macro_t;

char* preAssemble(FILE*, char*);
macro_t* findMacro(char*, node_t*);

bool isValidMacroName(char*);
node_t* addMacroNode(node_t*, char*);


#endif
