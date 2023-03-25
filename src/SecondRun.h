#ifndef SECONDRUN_HEADER
#define SECONDRUN_HEADER

#include "Includes.h"
#include "Types.h"


int second_run(int, int, node_t*, node_t*, node_t*, bool, FILE*, char*);


void createObjFile(int, word*, int, word*, char*);
void createEntryFile(node_t*, char*);
void createExternFile(node_t*, char*);

ERROR encodeArgumentToWord(char*, arg_type, word**, int, node_t*, node_t*);

bool checkForUndefinedEntries(node_t*, node_t*, char*);


#endif
