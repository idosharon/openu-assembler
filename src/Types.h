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

typedef struct {
    char name[REGISTER_NAME_SIZE];
    word value;
} register_t;

const register_t REGISTERS[REGISTER_COUNT] = {
        { "r0", {0} },
        { "r1", {0} },
        { "r2", {0} },
        { "r3", {0} },
        { "r4", {0} },
        { "r5", {0} },
        { "r6", {0} },
        { "r7", {0} }
};

/* addressing types */
typedef struct {

} addressing_t;


#endif
