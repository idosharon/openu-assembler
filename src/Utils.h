#ifndef UTILS_HEADER
#define UTILS_HEADER

#include "Includes.h"
#include "Consts.h"
#include "Types.h"

#define isStrEqual(a, b) ((strcmp(a, b)) == 0)

#define info(msg) printf("[info]: %s\n", msg)
#define info_file(msg, file_name) printf("[info]: %s: %s\n", msg, file_name)

#define encodeArgumentType(arg_type) (log2(arg_type))

#define getBitRepresentation(bit) ((bit) ? ONE_BIT_REPR : ZERO_BIT_REPR)

#define IS_DATA_SYMBOL(token) (isStrEqual(token, DATA_SYMBOL) || isStrEqual(token, STRING_SYMBOL))
#define IS_EXTERN_SYMBOL(token) (isStrEqual(token, EXTERN_SYMBOL))
#define IS_ENTRY_SYMBOL(token) (isStrEqual(token, ENTRY_SYMBOL))

#define resetArg(arg) (arg).type = None; (arg).value = NULL

bool is_number(char*);

int find_command(char*);

bool isValidLabel(char*);
label_t* findLabelInList(char *name, node_t *head);
label_t* findLabel(char*, node_t*, ...);
node_t* addLabelNode(node_t*, char*, size_t, label_type);

void updateDCInList(size_t, node_t*);
void updateDC(size_t, node_t*, ...);

void updateICInList(int, node_t*);
void updateIC(int, node_t*, ...);

bool isValidLabelFormat(char*);

arg_type getJumpParamType(char* , int);
int getJumpParamsLength(char*);
arg_type get_arg_type(char*, arg_type);
int find_register(char*);

char* getFileName(char*, char*);
FILE* openFile(char*, char*);

void writeBinToFile(size_t, size_t, FILE*);

void free_list(node_t*);

#endif
