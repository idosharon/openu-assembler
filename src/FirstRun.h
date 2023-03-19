#ifndef FIRSTRUN_HEADER
#define FIRSTRUN_HEADER

#include "Includes.h"
#include "Types.h"
#include <ctype.h>

#define IS_DATA_SYMBOL(token) (isStrEqual(token, DATA_SYMBOL) || isStrEqual(token, STRING_SYMBOL))
#define IS_EXTERN_SYMBOL(token) (isStrEqual(token, EXTERN_SYMBOL))
#define IS_ENTRY_SYMBOL(token) (isStrEqual(token, ENTRY_SYMBOL))

int firstRun(FILE*, char*);

void updateDCInList(int IC, node_t *head);
void updateDC(int, node_t*, ...);

char* getBinaryCommand(char*, char*, int);

bool deleteLabel(char*, node_t**);

#endif



