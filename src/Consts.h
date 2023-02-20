#ifndef CONSTS_HEADER
#define CONSTS_HEADER

/* File types */
#define ASM_FILE_EXTENSION ".as"
#define PRE_ASSEMBLER_FILE_EXTENSION ".am"

/* File reading consts */
#define MAX_LINE_SIZE 80

/* Memory units size */
#define BYTE_SIZE 7
#define WORD_SIZE (2 * BYTE_SIZE)
#define MEMORY_SIZE (256 * WORD_SIZE)

/* Registers consts */
#define REGISTER_SIZE WORD_SIZE
#define REGISTER_COUNT 8
#define REGISTER_NAME_SIZE 2

/* Symbols */
#define COMMENT_CHAR ';'
#define LINE_BREAK "\n"
#define SPACE_SEP " \t\r"LINE_BREAK

/* Macros */
#define START_MACRO_SYMBOL "mcr"
#define END_MACRO_SYMBOL "endmcr"

#endif
