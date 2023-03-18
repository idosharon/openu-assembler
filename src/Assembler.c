#include "Assembler.h"

int main(int argc, char* argv[]) {
    int file_index = 1;
    fopen("log.txt", "w");

    if (argc <= 1) {
        warn(FILE_NOT_SPECIFIED);
        return 1;
    } else {
        for (; file_index < argc; file_index++) {
            if(runAssembler(strdup(argv[file_index])) == -1) {
                return -1;
            }
        }
    }
    return 0;
}

int runAssembler(char* base_file_name) {
    char* current_file_name;
    FILE* current_file;

    /* get current_file name from argv */
    current_file_name = getFileName(base_file_name, ASM_FILE_EXTENSION);

    /* open the asm current_file */
    if ((current_file = openFile(current_file_name, "r"))) {
        info_file("Successfully Loaded", current_file_name);

        if((current_file_name = preAssemble(current_file, base_file_name))) {
            if((current_file = openFile(current_file_name, "r"))) {
                info_file("Successfully Loaded", current_file_name);
                firstRun(current_file, base_file_name);
            }

        }
    }

    return 0;
}
