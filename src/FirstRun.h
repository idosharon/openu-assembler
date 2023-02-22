#ifndef FIRSTRUN_HEADER
#define FIRSTRUN_HEADER

#include "Includes.h"
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

label_node_t* addLabelNode(label_node_t*, char*, int);


void firstRun(FILE*, char*);

int add_label(char*, label_node_t**);

#endif
