#ifndef CONSTS_HEADER
#define CONSTS_HEADER

/* File types */
#define ASM_FILE_EXTENSION ".as"
#define PRE_ASSEMBLER_FILE_EXTENSION ".am"
#define OBJ_FILE_EXTENSION ".o"

/* File reading consts */
#define MAX_LINE_SIZE 80

/* Memory units size */
#define BYTE_SIZE 7
#define WORD_SIZE (2 * BYTE_SIZE)
#define MEMORY_SIZE (256 * WORD_SIZE)
#define START_ADD 100
#define OPCODE_SIZE 4

/* Registers consts */
#define REGISTER_SIZE WORD_SIZE
#define REGISTER_COUNT 8
#define REGISTER_NAME_SIZE 2

/* Symbols */
#define COMMENT_CHAR ';'
#define LINE_BREAK "\n"
#define SPACE_SEP " \t\r"LINE_BREAK
#define COMMA_SEP ","SPACE_SEP

#define JMP_OPEN_BRACKET "("SPACE_SEP
#define JMP_CLOSE_BRACKET ")"SPACE_SEP


#define NUMBER_SYMBOL '#'

/* Labels */
#define LABEL_SEP ':'

/* Jump command format */
#define OPEN_BRACKET "("
#define CLOSE_BRACKET ")"
#define COMMA ','

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
/* register types */
typedef enum {
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
} REGISTERS_INDEX;
#endif
