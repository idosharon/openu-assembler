/*
 * File:        PreAssembler.h
 * Type:        Header file
 * Description: Pre Assembler header file, defines the pre assembler module functions headers and needed structs.
 *              Used by PreAssembler.c source file.
 *
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#ifndef PRE_ASSEMBLER_HEADER
#define PRE_ASSEMBLER_HEADER

/* includes */
#include "Types.h"
#include "Includes.h"

/* Struct: macro_t
 * Description: macro struct, holds macro name, macro data and lines count. */
typedef struct {
    /* macro name */
    char* name;
    /* macro data - pointer to string of the macro lines concatenated */
    char* data;
    /* lines count - number of lines in macro */
    int lines_count;
} macro_t;

/* function headers - explanation for each function can be found in the source file */
char* preAssemble(FILE*, char*);

/* macro functions */
macro_t* findMacro(char*, node_t*);
bool isValidMacroName(char*);
node_t* addMacroNode(node_t*, char*);


#endif
