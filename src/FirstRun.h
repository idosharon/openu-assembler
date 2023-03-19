#ifndef FIRSTRUN_HEADER
#define FIRSTRUN_HEADER

#include "Includes.h"
#include "SecondRun.h"
#include "Types.h"
#include <ctype.h>

int firstRun(FILE*, char*);

void updateDCInList(int IC, node_t *head);
void updateDC(int, node_t*, ...);

char* getBinaryCommand(char*, char*, int);

bool deleteLabel(char*, node_t**);

#endif



