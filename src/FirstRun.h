#ifndef FIRSTRUN_HEADER
#define FIRSTRUN_HEADER

#include "Includes.h"
#include "Types.h"
#include <ctype.h>

#define IS_DATA_SYMBOL(token) (isStrEqual(token, DATA_SYMBOL) || isStrEqual(token, STRING_SYMBOL))
#define IS_EXTERN_SYMBOL(token) (isStrEqual(token, EXTERN_SYMBOL))
#define IS_ENTRY_SYMBOL(token) (isStrEqual(token, ENTRY_SYMBOL))

typedef struct {
    char* name;
    int place;
    label_type type;
} label_t;

void firstRun(FILE*, char*);
label_t* findLabel(char*, node_t*);

char* getBinaryCommand(char*, char*, int);


bool isValidLabelName(char*);
node_t* addLabelNode(node_t*, char*, int , label_type);

#endif



