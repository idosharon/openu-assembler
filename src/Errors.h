#ifndef ERRORS_HEADER
#define ERRORS_HEADER

#include "Consts.h"

/* error messages */
enum {
    FILE_NOT_FOUND
} ERRORS_INDEX;

const char* ERRORS[] = {
    "File was not found"
};

#define error(error_index, file_name) (fprintf(stderr, "[error]: %s\n", ERRORS[error_index]))
#define line_error(error_index, line) (fprintf(stderr, "[error]: %s (line: %d)\n", ERRORS[error_index], line))
#define file_error(error_index, file_name) (fprintf(stderr, "[error]: %s (file: %s)\n", ERRORS[error_index], file_name))


/* warning messages */
enum {
    FILE_NOT_SPECIFIED
} WARNINGS_INDEX;

const char* WARNINGS[] = {
    "No file was specified"
};

#define warn(warning) (fprintf(stdout, "[warning]: %s\n", WARNINGS[warning]))
#define line_warning(warning, line) (fprintf(stdout, "[warning]: %s (line: %d)\n", WARNINGS[warning], line))


#endif
