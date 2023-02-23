#ifndef FIRSTRUN_HEADER
#define FIRSTRUN_HEADER

#include "Includes.h"
#include "Types.h"
#include <ctype.h>


typedef struct {
    char* name;
    int place;
    label_type type;
} label_t;

typedef struct {
    label_t* label;
    struct label_node_t* next;
} label_node_t;


void firstRun(FILE*, char*);
label_t* findLabel(char*, label_node_t*);

bool isValidLabelName(char*);
label_node_t * addLabelNode(label_node_t *, char*, int , label_type);

#endif



