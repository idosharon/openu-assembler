#ifndef FIRSTRUN_HEADER
#define FIRSTRUN_HEADER

#include "Includes.h"
#include "Types.h"
#include <ctype.h>

#define IS_DATA_SYMBOL(token) (is_equal(token, DATA_SYMBOL) || is_equal(token, STRING_SYMBOL))
#define IS_EXTERN_SYMBOL(token) (is_equal(token, EXTERN_SYMBOL))
#define IS_ENTRY_SYMBOL(token) (is_equal(token, ENTRY_SYMBOL))

typedef struct {
    char* name;
    int place;
    label_type type;
} label_t;

typedef struct {
    label_t* label;
    struct label_node_t* next;
} label_node_t;

void firstRun(FILE*, char*);
label_t* findLabel(char*, label_node_t*);

char* getBinaryCommand(char*, char*, int);

char* getBinaryMov(char*, char*, int);
char* getBinaryCmp(char*, char*, int);
char* getBinaryAdd(char*, char*, int);
char* getBinarySub(char*, char*, int);
char* getBinaryNot(char*, char*, int);
char* getBinaryClr(char*, char*, int);
char* getBinaryLea(char*, char*, int);
char* getBinaryInc(char*, char*, int);
char* getBinaryDec(char*, char*, int);
char* getBinaryJmp(char*, char*, int);
char* getBinaryBne(char*, char*, int);
char* getBinaryRed(char*, char*, int);
char* getBinaryPrn(char*, char*, int);
char* getBinaryJsr(char*, char*, int);
char* getBinaryRts(char*, char*, int);
char* getBinaryStop(char*, char*, int);

bool isValidLabelName(char*);
label_node_t * addLabelNode(label_node_t *, char*, int , label_type);

#endif



