#include "PreAssembler.h"

void pre_assemble(FILE* file, char* input_file_name) {
    FILE* pre_assembled_file;
    char line[MAX_LINE_SIZE];
    size_t line_number = 0;
    char* token;
    char* output_file_name = strdup(input_file_name);

    macro_node_t* macro_list = NULL;
    macro_t* current_macro = NULL;
    bool macro_flag = false;

    /* add pre assembler file extension */
    strcat(output_file_name, PRE_ASSEMBLER_FILE_EXTENSION);
    pre_assembled_file = fopen(output_file_name, "w");

    while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
        line_number++;
        printf("parsing line %lu: %s\n", line_number, line);

        /* skip comments */
        if(line[0] == COMMENT_CHAR) continue;

        /* split line into tokens */
        token = strtok(strdup(line), SPACE_STR);

        /* skip empty line */
        if(token[0] == LINE_BREAK[0]) continue;

        /* check if token is an existing macro */
        if((current_macro = find_macro(token, macro_list)) != NULL) {
            fputs(current_macro->data, file);
        } else {
            /* check for start of new macro def */
            if(strcmp(token, START_MACRO_SYMBOL) == 0) {
                current_macro = (macro_t*) (malloc(sizeof (macro_t)));
                current_macro->name = strtok(NULL, SPACE_STR""LINE_BREAK);

                if(find_macro(current_macro->name, macro_list) ||
                        strtok(NULL, SPACE_STR""LINE_BREAK) != NULL) {
                    fprintf(stderr, "[error]: %s (%s:%lu)\n", "Macro syntax error", input_file_name, line_number);
                }


                macro_flag = true;

            }


            fputs(line, pre_assembled_file);
        }
    }

    fclose(pre_assembled_file);
}

macro_t* find_macro(char* name, macro_node_t* head) {
    while(head != NULL) {
        if(strcmp(head->macro->name, name) == 0) {
            return head->macro;
        }
        head = head->next;
    }
    return NULL;
}
