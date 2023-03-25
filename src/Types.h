#ifndef TYPES_H
#define TYPES_H

#include "Consts.h"
#include <ctype.h>

/* define bool */
typedef enum { false, true } bool;

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

typedef struct {
    char* name;
    size_t place;
    label_type type;
} label_t;

/* command */
typedef struct {
    char* name;

    unsigned arg1_optional_types: 4;
    unsigned arg2_optional_types: 4;

} command_t;

/* binary string */
typedef struct {
    unsigned encoding_type: 2;
    unsigned dest_type: 2;
    unsigned src_type: 2;
    unsigned opcode: 4;
    unsigned first_par_type: 2;
    unsigned second_par_type: 2;
} binary_command;

/* binary data */
typedef struct {
    unsigned data: 14;
} binary_data;

typedef struct {
    unsigned encoding_type: 2;
    unsigned data: 12;
} binary_param;

typedef struct {
    unsigned encoding_type: 2;
    unsigned dest_register: 6;
    unsigned src_register: 6;
} binary_two_registers;

typedef union {
    binary_command command;
    binary_data data;
    binary_param param;
    binary_two_registers two_registers;
} word;

extern const command_t commands[NUM_OF_COMMANDS];




#endif
