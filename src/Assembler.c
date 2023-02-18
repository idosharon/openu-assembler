#include "Assembler.h"

int main(int argc, char* argv[]) {
    int file_index = 1;
    FILE* file;

    if (argc <= 1) {
        warn(FILE_NOT_SPECIFIED);
        return 1;
    } else {
        for (; file_index < argc; file_index++) {
            /* get file name from argv */
            char* file_name = strdup(argv[file_index]);

            /* currently assuming file name has no extension */
            /* TODO: add support for input with extension, for example: x.as instead of x */
            strcat(file_name, ASM_FILE_EXTENSION);

            /* open file */
            file = fopen(file_name, "r");

            if (file == NULL) {
                file_error(FILE_OPEN_ERROR, file_name);
            } else {
                printf("[status]: Successfully Loaded: %s\n", file_name);
                preAssemble(file, strdup(argv[file_index]));
            }
        }
    }
    return 0;
}
