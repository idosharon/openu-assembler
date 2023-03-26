/* File:  SecondRun.h
 * Type:  Header file
 * Description: Second run functions for the assembler.
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#ifndef SECONDRUN_HEADER
#define SECONDRUN_HEADER

#include "Includes.h"
#include "Types.h"

/* functions */
int second_run(size_t, size_t, node_t*, node_t*, node_t*, bool, FILE*, char*);

/* encode functions */
ERROR encodeArgumentToWord(char*, arg_type, word**, int, node_t*, node_t*);

/* entry and extern related functions */
void createEntryFile(node_t*, char*);
void createExternFile(node_t*, char*);
void createObjFile(size_t, word*, size_t, word*, char*);
bool checkForUndefinedEntries(node_t*, node_t*, char*);


#endif
