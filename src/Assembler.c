#include "Assembler.h"

int main(int argc, char* argv[]) {
    int file_index = 1;

    if (argc <= 1) {
        warn(FILE_NOT_SPECIFIED);
        return 1;
    } else {
        for (; file_index < argc; file_index++) {
            runAssembler(strdup(argv[file_index]));
        }
    }
    return 0;
}

int runAssembler(char* base_file_name) {
    char* current_file_name;
    FILE* file;

    /* get file name from argv */
    current_file_name = strdup(base_file_name);

    /* currently assuming file name has no extension */
    /* TODO: add support for input with extension, for example: x.as instead of x */
    strcat(current_file_name, ASM_FILE_EXTENSION);

    /* open the asm file */
    file = fopen(current_file_name, "r");

    if (file == NULL) {
        file_error(FILE_OPEN_ERROR, current_file_name);
    } else {
        printf("[info]: Successfully Loaded: %s\n", current_file_name);
        if((current_file_name = preAssemble(file, base_file_name))) {
            file = fopen(current_file_name, "r");
            if(file == NULL) {
                file_error(FILE_OPEN_ERROR, current_file_name);
                return -1;
            }

            firstRun(file, base_file_name);
        }
    }

    return 0;
}
