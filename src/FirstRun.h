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
    size_t place;
    label_type type;
} label_t;

int firstRun(FILE*, char*);
label_t* findLabelInList(char *name, node_t *head);
label_t* findLabel(char*, node_t*, node_t* , node_t*);

char* getBinaryCommand(char*, char*, int);

bool isValidLabel(char*);
bool deleteLabel(char*, node_t**);
node_t* addLabelNode(node_t*, char*, size_t, label_type);

int addLabel(char*, node_t*, size_t, label_type);

#endif



