#ifndef TYPES_H
#define TYPES_H

#include "Consts.h"

/* define bool */
typedef enum { false, true } bool;

typedef struct {
    unsigned bits: BYTE_SIZE;
} byte;

typedef struct {
    unsigned bits: WORD_SIZE;
} word;

/* label type */
typedef enum {
    Code,
    Data,
    Entry,
    Extern
} label_type;

/* memory type */
typedef enum {
    Absolute,
    External,
    Relocatable
} memory_type;

/* argument type */
typedef enum {
    Immediate,
    Direct,
    Jump,
    Register
} arg_type;

/* node type */
typedef struct {
    void* data;
    struct node_t* next;
} node_t;

/* command */
typedef struct {
    char* name;
    int args;
} command;

/* command type */
typedef struct {
    int type;
    int args : 3;

} command_type;
};




#endif
