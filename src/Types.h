/* File:   Types.h
 * Type:   Header file
 * Description: Types of variables used in the assembler.
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#ifndef TYPES_H
#define TYPES_H

#include "Consts.h"
#include <ctype.h>

/* Type: bool - enum
 * Description: Boolean type.
 */
typedef enum { false, true } bool;

/* Type: label_type - enum
 * Description: All the possible types of labels.
 * */
typedef enum {
    Code,
    Data,
    Entry,
    Extern
} label_type;

/* Type: memory_type - enum
 * Description: All the possible types of memory.
 * */
typedef enum {
    Absolute,
    External,
    Relocatable
} memory_type;

/* Type: arg_type - enum
 * Description: All the possible types of arguments, ordered by their bit representation in order to use bit operations on them.
 *              in order to get the argument type for the command, use the log2 function on the argument type.
 */
typedef enum {
    None = 0,       /* 0000 = 0 */
    Immediate = 1,  /* 0001 = 0 */
    Direct = 2,     /* 0010 = 1 */
    Jump = 4,       /* 0100 = 2 */
    Register = 8    /* 1000 = 3 */
} arg_type;

/* Type: node_t - struct
 * Description: Node of a linked list.
 * */
typedef struct {
    void* data;
    struct node_t* next;
} node_t;

/* Type: label_t - struct
 * Description: Label type that holds its name, place and type.
 * */
typedef struct {
    char* name;
    size_t place;
    label_type type;
} label_t;

/* Type: command_t - struct
 * Description: Command type that holds its name and the possible types of its arguments.
 */
typedef struct {
    char* name;

    unsigned arg1_optional_types: 4;
    unsigned arg2_optional_types: 4;
} command_t;

/* Type: binary_command - struct
 * Description: The binary format of a command.
 */
typedef struct {
    unsigned encoding_type: 2;
    unsigned dest_type: 2;
    unsigned src_type: 2;
    unsigned opcode: 4;
    unsigned first_par_type: 2;
    unsigned second_par_type: 2;
} binary_command;

/* Type: binary_data - struct
 * Description: The binary format of a data.
 */
typedef struct {
    unsigned data: 14;
} binary_data;

/* Type: binary_param - struct
 * Description: The binary format of a parameter.
 */
typedef struct {
    unsigned encoding_type: 2;
    unsigned data: 12;
} binary_param;

/* Type: binary_two_registers - struct
 * Description: The binary format of two registers.
 */
typedef struct {
    unsigned encoding_type: 2;
    unsigned dest_register: 6;
    unsigned src_register: 6;
} binary_two_registers;

/* Type: word - union
 * Description: A word in the binary file.
 */
typedef union {
    binary_command command;
    binary_data data;
    binary_param param;
    binary_two_registers two_registers;
} word;

/* set of all the commands */
extern const command_t commands[NUM_OF_COMMANDS];

#endif
