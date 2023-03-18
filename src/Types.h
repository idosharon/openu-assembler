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

typedef enum {
    Code,
    Data,
    Entry,
    Extern
} label_type;

typedef enum {
    Absolute,
    External,
    Relocatable
} memory_t;

typedef struct {
    char* base_file_name;
    char* asm_file;
    char* pre_asm_file;
} Files;

typedef struct {
    void* data;
    struct node_t* next;
} node_t;



#endif
