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
    MEMORY_OVERFLOW
} ERROR;

extern const char* ERRORS[];

#define error(error_index) (printf("[error]: %s\n", ERRORS[error_index]))
#define line_error(error_index, file_name, line) (printf("[error]: %s (file: %s, line: %lu)\n", ERRORS[error_index], file_name, line))
#define file_error(error_index, file_name) (printf("[error]: %s (file: %s)\n", ERRORS[error_index], file_name))

/* warning messages */
typedef enum {
    FILE_NOT_SPECIFIED,
    MACRO_SYNTAX_WARNING,
    LABEL_DEF_BEFORE_EXTERN,
    LABEL_DEF_BEFORE_ENTRY
} WARNING;

extern const char* WARNINGS[];

#define warn(warning) (printf("[warning]: %s\n", WARNINGS[warning]))
#define line_warning(warning_index, file_name, line) (printf("[warning]: %s (file: %s, line: %lu)\n", WARNINGS[warning_index], file_name, line))
#define file_warning(warning_index, file_name) (printf("[warning]: %s (file: %s)\n", WARNINGS[warning_index], file_name))


#endif
