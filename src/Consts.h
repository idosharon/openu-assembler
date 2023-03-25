/* File:  Consts.h
 * Type:   Header file
 * Description: Constants used in the assembler.
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#ifndef CONSTS_HEADER
#define CONSTS_HEADER

/* File types */
#define ASM_FILE_EXTENSION ".as"
#define PRE_ASSEMBLER_FILE_EXTENSION ".am"
#define OBJ_FILE_EXTENSION ".o"
#define ENTRY_FILE_EXTENSION ".ent"
#define EXTERN_FILE_EXTENSION ".ext"

/* File reading consts */
#define MAX_LINE_SIZE   80
#define FILE_READ_MODE  "r"
#define FILE_WRITE_MODE "w"

/* Memory units size */
#define BYTE_SIZE 7
#define WORD_SIZE (2 * BYTE_SIZE)

#define MAX_MEMORY_WORDS 256
#define MAX_MEMORY_SIZE (MAX_MEMORY_WORDS * WORD_SIZE)


#define START_ADD 100
#define OPCODE_SIZE 4
#define MAX_DATA_VALUE (8191)
#define MIN_DATA_VALUE (-8192)
#define MAX_PARAM_VALUE (2047)
#define MIN_PARAM_VALUE (-2048)

/* Symbols */
#define COMMENT_CHAR ';'
#define LINE_BREAK "\n\0"
#define SPACE_SEP " \t\r"LINE_BREAK
#define COMMA_SEP ","SPACE_SEP
#define STRING_QUOTE '"'
#define NULL_TERMINATOR '\0'

#define IMMEDIATE_PREFIX '#'

#define JMP_OPEN_BRACKET "("SPACE_SEP
#define JMP_CLOSE_BRACKET ")"SPACE_SEP

#define ONE_BIT_REPR '1'
#define ZERO_BIT_REPR '0'

/* Labels */
#define LABEL_SEP ':'

/* Jump to command format */
#define OPEN_BRACKET "("

/* Macros */
#define START_MACRO_SYMBOL "mcr"
#define END_MACRO_SYMBOL "endmcr"

/* Keywords */
#define DATA_SYMBOL     ".data"
#define STRING_SYMBOL   ".string"
#define ENTRY_SYMBOL    ".entry"
#define EXTERN_SYMBOL   ".extern"


/* All commands */
#define NUM_OF_COMMANDS 16

/* command types */
#define NUM_OF_REGISTERS 8
extern const char* registers[NUM_OF_REGISTERS];

#endif
