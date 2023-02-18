#ifndef UTILS_HEADER
#define UTILS_HEADER

#include "Includes.h"
#include "Consts.h"
#include "Types.h"

const register_t findRegister(char* name);

char* next_token(char* str, const char* delim);


#endif
