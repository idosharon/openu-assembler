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
    None = 0,       /* 0000 = 0 */
    Immediate = 1,  /* 0001 = 0 */
    Direct = 2,     /* 0010 = 1 */
    Jump = 4,       /* 0100 = 2 */
    Register = 8    /* 1000 = 3 */
} arg_type;

/* node type */
typedef struct {
    void* data;
    struct node_t* next;
} node_t;

/* command */
typedef struct {
    char* name;

    unsigned arg1: 4;
    unsigned arg2: 4;

    enum {
        ZERO_ARGS,
        ONE_ARGS,
        TWO_ARGS,
        THREE_ARGS
    } num_of_args;

    bool is_jump;

} command_t;

extern const command_t commands[NUM_OF_COMMANDS];




#endif
