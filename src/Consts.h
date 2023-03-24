#ifndef CONSTS_HEADER
#define CONSTS_HEADER

/* File types */
#define ASM_FILE_EXTENSION ".as"
#define PRE_ASSEMBLER_FILE_EXTENSION ".am"
#define OBJ_FILE_EXTENSION ".o"
#define ENT_FILE_EXTENSION ".ent"
#define EXT_FILE_EXTENSION ".ext"

/* File reading consts */
#define MAX_LINE_SIZE 80
#define FILE_READ_MODE "r"
#define FILE_WRITE_MODE "w"

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
#define STRING_QUOTE '"'
#define NULL_TERMINATOR '\0'

#define IMMEDIATE_PREFIX '#'

#define JMP_OPEN_BRACKET "("SPACE_SEP
#define JMP_CLOSE_BRACKET ")"SPACE_SEP

#define ONE_BIT_REPR '1'
#define ZERO_BIT_REPR '0'


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

#define EMPTY_ARGUMENT {NULL, None}

#endif
