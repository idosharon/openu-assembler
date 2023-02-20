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
    Absolute,
    External,
    Relocatable
} memory_t;


#endif
