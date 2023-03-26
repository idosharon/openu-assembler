/*
 * File:        PreAssembler.c
 * Type:        Source file
 * Description: Pre Assembler module, handles macro definitions.
 *              Creates a new file with the macros expanded X.am file.
 *
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#include "PreAssembler.h"

/* Function: preAssemble
 * Description: pre-assemble the file, list all its macros and expand them in a new file with .am extension.
 * Input: file - pointer to file to pre-assemble,
 *        base_file_name - base file name (excluding extension, used for output file name with new .am extension)
 * Output: output_file_name - name of the output file with the pre-assembled file (with .am extension)
 * Example: preAssemble(file, "test") -> "test.am"
 */
char* preAssemble(FILE* file, char* base_file_name) {
    /* open pre assembler file */
    FILE* pre_assembled_file;
    char* pre_assembled_file_name = getFileName(base_file_name, PRE_ASSEMBLER_FILE_EXTENSION);

    /* line buffer & line number */
    char* line = (char*) calloc(MAX_LINE_SIZE, sizeof (char));
    size_t line_number = 0;

    /* token pointer */
    char* token;

    /* macro list & macro flag */
    node_t* macro_list = NULL;
    macro_t* current_macro = NULL;
    bool macro_flag = false;

    /* add pre assembler file extension & start writing to file */
    if(!(pre_assembled_file = openFile(pre_assembled_file_name, FILE_WRITE_MODE))) return NULL;

    /* read new line from file */
    while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
        /* increase line counter */
        line_number++;

        /* split line into tokens */
        token = strtok(strdup(line), SPACE_SEP);

        /* skip comment & empty line */
        if(token == NULL || token[0] == COMMENT_CHAR) continue;

        /* check if in macro def */
        if(macro_flag) {
            /* check for end of macro def */
            if(strcmp(token, END_MACRO_SYMBOL) == 0) {
                /* check for extra tokens */
                if(strtok(NULL, SPACE_SEP)) {
                    line_error(MACRO_SYNTAX_ERROR, base_file_name, line_number, line);
                }
                macro_flag = false;
            } else {
                /* add line to macro */
                (current_macro->lines_count)++;
                current_macro->data = (char*) realloc(current_macro->data,
                                                      current_macro->lines_count * MAX_LINE_SIZE);
                strcat(current_macro->data, line);
            }
        } else {
            /* check if token is an existing macro */
            if((current_macro = findMacro(token, macro_list)) != NULL) {
                if (strtok(NULL,SPACE_SEP))
                    line_warning(MACRO_SYNTAX_WARNING,base_file_name,line_number);
                fputs(current_macro->data, pre_assembled_file);
            } else {
                /* check for start of new macro def */
                if(strcmp(token, START_MACRO_SYMBOL) == 0) {
                    /* get macro name */
                    token = strtok(NULL, SPACE_SEP);

                    if(!isValidMacroName(token) ||
                            findMacro(token, macro_list) ||
                                strtok(NULL, SPACE_SEP)) {
                        line_error(MACRO_SYNTAX_ERROR, base_file_name, line_number, line);
                    } else {
                        /* create new macro */
                        macro_flag = true;
                        macro_list = addMacroNode(macro_list, token);
                        current_macro = (macro_t *) macro_list->data;
                    }

                } else {
                    /* write line to pre-assembled file */
                    fputs(line, pre_assembled_file);
                }
            }
        }
    }

    /* check if macro def is still open */
    if(macro_flag) {
        file_error(MACRO_SYNTAX_ERROR, base_file_name);
    }

    fclose(pre_assembled_file);

    info_file("Created pre assembled file", pre_assembled_file_name);

    /* free all variables */
    free_list(macro_list);
    free(line);

    return pre_assembled_file_name;
}

/* Function: findMacro
 * Description: find a macro in a given list of macros
 * Input: name - char pointer to the name of the macro to find
 *        head - pointer to the head of the list of macros
 * Output: pointer to the macro if found, NULL otherwise
 * Example: macro_list: MACRO1 -> MACRO2 -> MACRO3
 *          findMacro("MACRO2", macro_list) -> pointer to MACRO2 node as macro_t*
 */
macro_t* findMacro(char* name, node_t* head) {
    /* current macro */
    macro_t* macro;
    /* iterate over the given list until NULL */
    while(head != NULL) {
        /* check if macro name is equal to the given name */
        if(isStrEqual((macro = (macro_t*) head->data)->name, name)) {
            return macro;
        }
        /* move to next node */
        head = (node_t*) head->next;
    }
    return NULL;
}

/* Function: isValidMacroName
 * Description: check if a given macro name is valid
 * Input: macro_name - char pointer to the name of the macro to check
 * Output: true if valid, false otherwise
 * Example: isValidMacroName("MACRO1") -> true
 *          isValidMacroName("MACRO_1") -> true
 *          isValidMacroName("MACRO-1") -> false
 */
bool isValidMacroName(char* macro_name) {
    /* check if macro name is valid - only contains alphabetic and numbers */
    if (!isalpha(*macro_name))
        return false;
    for(; *macro_name != '\0'; macro_name++) {
        if( !isalpha(*macro_name) && !isdigit(*macro_name) )
            return false;
    }
    return true;
}

/* Function: addMacroNode
 * Description: add a new macro node to the start of a given list of macros
 * Input: head - pointer to the head of the list of macros
 *        name - char pointer to the name of the new macro
 * Output: pointer to the new head of the list of macros
 * Example: macro_list: MACRO1 -> MACRO2 -> MACRO3
 *          addMacroNode(macro_list, "MACRO4"): MACRO4 -> MACRO1 -> MACRO2 -> MACRO3
 */
node_t* addMacroNode(node_t* head, char* name) {
    /* set current macro to the new macro */
    macro_t* new_macro = (macro_t*) (malloc(sizeof (macro_t)));
    node_t* new_node = (node_t*) (malloc(sizeof (node_t)));

    /* set macro name and allocate its data */
    new_macro->name = name;
    new_macro->data = (char*) calloc(1,sizeof (char));
    new_macro->lines_count = 0;

    /* create new macro node and append it to the start of list */
    new_node->data = new_macro;
    new_node->next = (struct node_t*) head;
    return new_node;
}
