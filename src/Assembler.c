/* File: Assembler.c
 * Type: Source file
 * Description: The main file of the assembler
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#include "Assembler.h"

/*      == Main Function ==
 * Description: runs the assembler on the given files from the command line
 * Parameters: argc - the number of arguments (given from system)
 *             argv - the arguments (given from system)
 * Returns: number of files that got errors, 0 if no errors occurred
 */
int main(int argc, char* argv[]) {
    /* index of the current file in argv starting from 1 because the program takes first arg */
    int file_index = 1;
    int error_counter = 0;

    /* check if file was specified */
    if (argc <= 1) {
        warn(FILE_NOT_SPECIFIED);
        return ERROR_CODE;
    } else {
        /* run assembler on each file */
        for (; file_index < argc; file_index++) {
            /* run assembler on current file, and increase error counter if error is returned (-1) */
            error_counter += runAssembler(strdup(argv[file_index]));
            printf("\n");
        }
    }

    return error_counter;
}

/* Function: runAssembler
 * Description: runs the assembler on the given file
 * Parameters: base_file_name - the name of the file to run the assembler on (without the extension)
 * Returns: 0 if no errors occurred, 1 otherwise
 */
int runAssembler(char* base_file_name) {
    char* current_file_name;
    FILE* current_file;

    /* get current_file name from argv */
    current_file_name = getFileName(base_file_name, ASM_FILE_EXTENSION);

    /* print file name */
    info_file("Running assembler for", current_file_name);

    /* open the asm current_file */
    if ((current_file = openFile(current_file_name, FILE_READ_MODE))) {
        if((current_file_name = preAssemble(current_file, base_file_name))) {
            if((current_file = openFile(current_file_name, FILE_READ_MODE))) {
                return firstRun(current_file, base_file_name);
            }
        }
    }

    /* free memory */
    free(current_file_name);
    fclose(current_file);

    return 1;
}
