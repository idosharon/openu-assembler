#include "PreAssembler.h"

char* preAssemble(FILE* file, char* base_file_name) {
    /* open pre assembler file */
    FILE* pre_assembled_file;
    char* output_file_name = strdup(base_file_name);
    /* line buffer & line number */
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof (char));
    size_t line_number = 0;
    /* token pointer */
    char* token;

    /* macro list & macro flag */
    macro_node_t* macro_list = NULL;
    macro_node_t* next_node = macro_list;
    macro_t* current_macro = NULL;
    bool macro_flag = false;

    /* add pre assembler file extension & start writing to file */
    strcat(output_file_name, PRE_ASSEMBLER_FILE_EXTENSION);
    pre_assembled_file = fopen(output_file_name, "w");
    if(pre_assembled_file == NULL) {
        file_error(FILE_OPEN_ERROR, output_file_name);
        return NULL;
    }

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
                    line_error(MACRO_SYNTAX_ERROR, base_file_name, line_number);
                }
                macro_flag = false;
            } else {
                /* add line to macro */
                (current_macro->lines_count)++;
                current_macro->data = (char*)realloc(current_macro->data,current_macro->lines_count*MAX_LINE_SIZE);
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
                        line_error(MACRO_SYNTAX_ERROR, base_file_name, line_number);
                    } else {
                        /* create new macro */
                        macro_flag = true;
                        macro_list = addMacroNode(macro_list, token);
                        current_macro = macro_list->macro;
                    }

                } else {
                    /* write line to pre-assembled file */
                    fputs(line, pre_assembled_file);
                }
            }
        }
    }

    /* free all variables */
    while(macro_list != NULL) {
        next_node = (macro_node_t *) macro_list->next;
        free(macro_list->macro->data);
        free(macro_list->macro);
        free(macro_list);
        macro_list = next_node;
    }

    free(line);

    if(macro_flag) {
        file_error(MACRO_SYNTAX_ERROR, base_file_name);
    }

    fclose(pre_assembled_file);
    printf("[info]: Created Pre Assembled file: %s\n", output_file_name);

    return output_file_name;
}

macro_t* findMacro(char* name, macro_node_t* head) {
    while(head != NULL) {
        if(strcmp(head->macro->name, name) == 0) {
            return head->macro;
        }
        head = (macro_node_t *) head->next;
    }
    return NULL;
}

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

macro_node_t* addMacroNode(macro_node_t* head, char* name) {
    /* set current macro to the new macro */
    macro_t* new_macro = (macro_t*) (malloc(sizeof (macro_t)));
    macro_node_t* new_macro_node = (macro_node_t*) (malloc(sizeof (macro_node_t)));

    /* set macro name and allocate its data */
    new_macro->name = name;
    new_macro->data = (char*) calloc(1,sizeof (char));
    new_macro->lines_count = 0;

    /* create new macro node and append it to the start of list */
    new_macro_node->macro = new_macro;
    new_macro_node->next = (struct macro_node_t *) head;
    return new_macro_node;
}
