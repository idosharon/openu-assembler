#ifndef SECONDRUN_HEADER
#define SECONDRUN_HEADER

#include "Includes.h"
#include "Types.h"


int second_run(int, int, node_t*, node_t*, node_t*, bool, FILE*, char*);


void createObjFile(int, word*, int, word*, char*);
void createEntryFile(node_t*, char*);
void createExternFile(node_t*, char*);

ERROR encodeArgumentToWord(char*, word**, int, arg_type, node_t*, node_t*, node_t**, int);

bool checkForUndefinedEntries(node_t*, node_t*, char*);


#endif
