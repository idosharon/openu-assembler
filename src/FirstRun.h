#include "Includes.h"
#include <ctype.h>

#ifndef OPENU_ASSEMBLER_FIRSTRUN_H
#define OPENU_ASSEMBLER_FIRSTRUN_H

#endif //OPENU_ASSEMBLER_FIRSTRUN_H


typedef struct {
    char* name;
    int place;
} label_t;

typedef struct {
    label_t* label;
    struct label_node_t* next;
} label_node_t;

label_node_t* addLabelNode(label_node_t*, char*, int);


void first_run(FILE*, char*);

int add_label(char**, label_node_t*);