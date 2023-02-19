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

/*
typedef struct {
    char name[REGISTER_NAME_SIZE];
} register_t;

const register_t REGISTERS[REGISTER_COUNT] = {
        { "r0"},
        { "r1"},
        { "r2"},
        { "r3"},
        { "r4"},
        { "r5"},
        { "r6"},
        { "r7"}
};
*/



#endif
