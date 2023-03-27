/*
 * File:   Utils.h
 * Type:   Header file
 * Description: Utility functions for the assembler.
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#ifndef UTILS_HEADER
#define UTILS_HEADER

#include "Includes.h"
#include "Consts.h"
#include "Types.h"

/* Macros */

/* compare string macros */
#define isStrEqual(a, b) ((strcmp(a, b)) == 0)

/* compare to symbols */
#define IS_DATA_SYMBOL(token) (isStrEqual(token, DATA_SYMBOL) || isStrEqual(token, STRING_SYMBOL))
#define IS_EXTERN_SYMBOL(token) (isStrEqual(token, EXTERN_SYMBOL))
#define IS_ENTRY_SYMBOL(token) (isStrEqual(token, ENTRY_SYMBOL))

/* log macros */
#define info_file(msg, file_name) printf("[info]: %s: %s\n", msg, file_name)

/* Macro: encodeArgumentType
 * Description: encode argument type to bit representation using log2 function
 * Takes: arg_type - argument type (arg_type)
 * Returns: a bit representation of argument type (int)
 * Example: encodeArgumentType(Immediate) = 0 (log2(Immediate = 1) = 0)
 *          encodeArgumentType(Direct) = 1 (log2(Direct = 2) = 1)
 * */
#define encodeArgumentType(arg_type) (log2(arg_type))
/* change 1 and 0 to / and . */
#define getBitRepresentation(bit) ((bit) ? ONE_BIT_REPR : ZERO_BIT_REPR)

/* Macro: resetParam
 * Description: reset parameter data and encoding type
 * Takes: param - pointer to parameter (param_t*)
 * */
#define resetParam(param) param->data = 0; param->encoding_type = None

#define isDataInRange(num) (num >= MIN_DATA_VALUE && num <= MAX_DATA_VALUE)
#define isParamInRange(num) (num >= MIN_PARAM_VALUE && num <= MAX_PARAM_VALUE)

/* Functions */

/* command functions */
int find_command(char*);

/* label functions */
bool isValidLabel(char*);
bool isValidLabelFormat(char*);

/* label list functions */
label_t* findLabelInList(char *name, node_t *head);
label_t* findLabel(char*, node_t*, ...);
node_t* addLabelNode(node_t*, char*, size_t, label_type);

/* DC update functions */
void updateDCInList(size_t, node_t*);
void updateDC(size_t, node_t*, ...);

/* IC update functions */
void updateICInList(int, node_t*);
void updateIC(int, node_t*, ...);

/* jump with params command functions */
arg_type getJumpParamType(char* , int);
int getJumpParamsLength(char*);

/* argument functions */
arg_type get_arg_type(char*, arg_type);

/* register functions */
int find_register(char*);

/* file functions */
char* getFileName(char*, char*);
FILE* openFile(char*, char*);
void writeObjToFile(size_t, size_t, FILE*);

/* general list functions */
void free_list(node_t*);

/* general utils functions */
bool is_number(char*);
ERROR to_number(char*, int*);

#endif
