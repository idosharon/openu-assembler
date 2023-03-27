/* File: Errors.h
 * Type: Header file
 * Description: Header file for the errors module
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#ifndef ERRORS_HEADER
#define ERRORS_HEADER

/* error messages */
typedef enum {
    /* general error code */
    ERROR_CODE = -1,

    /* no error code */
    NO_ERROR,

    /* specific error codes with name in ERRORS array */
    FILE_OPEN_ERROR,
    MACRO_SYNTAX_ERROR,
    LABEL_SYNTAX_ERROR,
    MULTIPLE_LABEL_DEFINITIONS,
    DATA_SYNTAX_ERROR,
    EXTERN_MISSING_ARGUMENT,
    ENTRY_MISSING_ARGUMENT,
    COMMAND_NOT_FOUND,
    INVALID_DEST_ARG,
    INVALID_SOURCE_ARG,
    PRESERVED_KEYWORD,
    TOO_MANY_ARGS,
    TOO_FEW_ARGS,
    EXTERN_TOO_MANY_ARGUMENTS,
    ENTRY_TOO_MANY_ARGUMENTS,
    UNDEFINED_LABEL,
    MULTIPLE_EXTERN_CALLS,
    MULTIPLE_ENTRY_CALLS,
    CONFLICT_LOCAL_EXTERNAL_LABELS,
    STRING_MISSING_ARGUMENT,
    STRING_SYNTAX_ERROR,
    PARAM_VALUE_OUT_OF_RANGE,
    MISSING_CODE_AFTER_LABEL,
    STRING_MISSING_QUOTE,
    DATA_OUT_OF_RANGE,
    COMMAND_OR_DATA_INSTRUCTION_NOT_FOUND,
    MEMORY_OVERFLOW,
    CONSECUTIVE_LABELS,
    LABEL_TOO_LONG
} ERROR;

/* array of error messages */
extern const char* ERRORS[];

/* Macros for printing error messages */
#define error(error_index) (printf("[error]: %s\n", ERRORS[error_index]))
#define line_error(error_index, base_file_name, line_number, line) if(strchr(line, LINE_TERMINATOR) != NULL) { *strchr(line, LINE_TERMINATOR) = NULL_TERMINATOR; } \
                                                                    printf("[error]: %s (see file: %s%s, line: %d): %s\n", ERRORS[error_index], base_file_name, PRE_ASSEMBLER_FILE_EXTENSION, line_number, line)
#define file_error(error_index, base_file_name) (printf("[error]: %s (see file: %s%s)\n", ERRORS[error_index], base_file_name, ASM_FILE_EXTENSION))

/* warning messages */
typedef enum {
    FILE_NOT_SPECIFIED,
    MACRO_SYNTAX_WARNING,
    LABEL_DEF_BEFORE_EXTERN,
    LABEL_DEF_BEFORE_ENTRY
} WARNING;

/* array of warning messages */
extern const char* WARNINGS[];

/* Macros for printing warning messages */
#define warn(warning) (printf("[warning]: %s\n", WARNINGS[warning]))
#define line_warning(warning_index, base_file_name, line_number, line) if(strchr(line, LINE_TERMINATOR) != NULL) { *strchr(line, LINE_TERMINATOR) = NULL_TERMINATOR; } \
                                                                       printf("[warning]: %s (file: %s%s, line: %d): %s\n", WARNINGS[warning_index], base_file_name, PRE_ASSEMBLER_FILE_EXTENSION, line_number, line)


#endif
