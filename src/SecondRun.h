#ifndef SECONDRUN_HEADER
#define SECONDRUN_HEADER

#include "Includes.h"
#include "Types.h"


int second_run(int, int, node_t*, node_t*, node_t*, bool, FILE*, char*);


void createObjFile(int, word*, int, word*, char*);
void createEntryFile(node_t*, char*);
void createExternFile(node_t*, char*);

word* encodeArgumentToWord(argument_t, argument_t, node_t*, node_t*, char*, size_t);


#endif
