/* File:        FirstRun.c
 * Type:        Source file
 * Description: First run module, handles all first run and continues to second run.
 *
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#include "FirstRun.h"

/* Function: firstRun
 * Description: first run of the assembler, handles all first run and continues to second run.
 * Input: file - pointer to file to assemble,
 *        base_file_name - base file name (excluding extension, used for output file name with new .ob extension)
 * Output: 0 - if no errors occurred
 *         1 - if errors occurred
 */
int firstRun(FILE* file, char* base_file_name) {
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof (char));
    size_t line_number = 0;
    size_t command_index = 0;
    int command_length = 1;
    command_t command;
    arg_type source_type = None, dest_type = None;
    char* token;

    /* pointers to start and finish of string in .string*/
    char* first_quote, *last_quote;

    /* create list for labels */
    node_t* label_list = NULL;

    char* current_label = NULL;
    bool label_flag = false;

    /* error flag */
    bool error_flag = false;

    /* create list for external and entry labels */
    node_t* extern_list = NULL;
    node_t* entry_list = NULL;

    /* init IC and DC */
    size_t IC = START_ADD;
    size_t DC = 0;

    /* read new line from file */
    while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
        /* increase line counter */
        line_number++;
        /* split line into tokens */
        token = strtok(strdup(line), SPACE_SEP);

        label_flag = false;

        /* check if there is a label, TODO: improve maybe move to other function */
        if(strchr(line, LABEL_SEP)) {
            /* check if label is valid */
            if (isValidLabel(token)) {
                label_flag = true;
                current_label = token;
                current_label[strlen(current_label)-1] = NULL_TERMINATOR;
                token = strtok(NULL, SPACE_SEP);

                if (token == NULL) {
                    line_error(MISSING_CODE_AFTER_LABEL, base_file_name, line_number, line);
                    error_flag = true;
                    continue;
                }

                /* check if label exists in future labels */
                if (findLabelInList(current_label, label_list)) {
                    line_error(MULTIPLE_LABEL_DEFINITIONS, base_file_name, line_number, line);
                    error_flag = true;
                    continue;
                }
            } else {
                /* label is not valid */
                line_error(LABEL_SYNTAX_ERROR, base_file_name, line_number, line);
                error_flag = true;
                continue;
            }
        }

        /* check all symbol types */
        if (IS_DATA_SYMBOL(token)) {
            /* check if label is already defined */
            if (label_flag) {
                label_list = addLabelNode(label_list, current_label, DC, Data);
            }
            /* check for .data symbol */
            if (isStrEqual(token, DATA_SYMBOL)) {
                /* calculate data length */
                while((token = strtok(NULL, COMMA_SEP)) != NULL) {
                    if (is_number(token)) {
                        /* TODO: move atoi to value */
                        if(!isDataInRange(atoi(token))) {
                            line_error(DATA_OUT_OF_RANGE, base_file_name, line_number, line);
                            error_flag = true;
                            continue;
                        }
                        DC++;
                    } else {
                        line_error(DATA_SYNTAX_ERROR, base_file_name,line_number, line);
                        error_flag = true;
                        continue;
                    }
                }
            }
            else if (isStrEqual(token, STRING_SYMBOL)) {
                token = strtok(NULL,SPACE_SEP);
                first_quote = strchr(line, STRING_QUOTE);
                last_quote = strrchr(line, STRING_QUOTE);
                if (token == NULL) {
                    line_error(STRING_MISSING_ARGUMENT, base_file_name,line_number, line);
                    error_flag = true;
                    continue;
                }
                if (last_quote == NULL || first_quote == NULL || last_quote <= first_quote) {
                    line_error(STRING_MISSING_QUOTE, base_file_name, line_number, line);
                    error_flag = true;
                    continue;
                }
                if (strtok(last_quote+1,SPACE_SEP) != NULL) {
                    line_error(STRING_SYNTAX_ERROR, base_file_name,line_number, line);
                    error_flag = true;
                    continue;
                }
                strncpy(token, first_quote, last_quote-first_quote);
                token[last_quote-first_quote+1] = NULL_TERMINATOR;
                if (token[0] != STRING_QUOTE || token[strlen(token)-1] != STRING_QUOTE) {
                    line_error(STRING_SYNTAX_ERROR, base_file_name,line_number, line);
                    error_flag = true;
                    continue;
                }
                DC += strlen(token)-1;
            }
        }
        else if (IS_EXTERN_SYMBOL(token) || IS_ENTRY_SYMBOL(token)) {

            /* case: extern symbol */
            if(IS_EXTERN_SYMBOL(token)) {
                /* get external label */
                token = strtok(NULL,SPACE_SEP);

                /* if not found raise error */
                if (token == NULL) {
                    line_error(EXTERN_MISSING_ARGUMENT,base_file_name,line_number, line);
                    error_flag = true;
                    continue;
                }
                /* if more than one found raise error */
                if(strtok(NULL,SPACE_SEP) != NULL) {
                    line_error(EXTERN_TOO_MANY_ARGUMENTS, base_file_name,line_number, line);
                    error_flag = true;
                    continue;
                }
                /* if the label is not valid raise error */
                if (!isValidLabel(token)) {
                    line_error(LABEL_SYNTAX_ERROR,base_file_name,line_number, line);
                    error_flag = true;
                    continue;
                }

                /* if the label is already called from external file raise error */
                if (findLabelInList(token, extern_list)) {
                    line_error(MULTIPLE_EXTERN_CALLS, base_file_name, line_number, line);
                    error_flag = true;
                    continue;
                }
                /* add to external list */

                if (label_flag) {
                    line_warning(LABEL_DEF_BEFORE_EXTERN, base_file_name, line_number);
                }
                extern_list = addLabelNode(extern_list, token, line_number, Extern);
            } else {

                /* get entry label */
                token = strtok(NULL,SPACE_SEP);

                /* if no token found raise error */
                if (token == NULL) {
                    line_error(ENTRY_MISSING_ARGUMENT,base_file_name,line_number, line);
                    error_flag = true;
                    continue;
                }
                /* if more than one found raise error */
                if(strtok(NULL,SPACE_SEP) != NULL) {
                    line_error(ENTRY_TOO_MANY_ARGUMENTS, base_file_name,line_number, line);
                    error_flag = true;
                    continue;
                }
                /* if label is not valid raise error */
                if (!isValidLabel(token)) {
                    line_error(LABEL_SYNTAX_ERROR,base_file_name,line_number, line);
                    error_flag = true;
                    continue;
                }

                /* if label is already defined raise error */
                if (findLabelInList(token, entry_list)) {
                    line_error(MULTIPLE_ENTRY_CALLS, base_file_name, line_number, line);
                    error_flag = true;
                    continue;
                }

                if (label_flag) {
                    line_warning(LABEL_DEF_BEFORE_ENTRY, base_file_name, line_number);
                }
                /* add to entry list */
                entry_list = addLabelNode(entry_list, token, line_number, Entry);
            }
        }
        else {
            /* get current command index */
            if ((command_index = find_command(token)) != ERROR_CODE) {

                /* add label if needed */
                if (label_flag) {
                    label_list = addLabelNode(label_list, current_label, IC, Code);
                }

                /* check command type (group) */
                command_length = 1;
                command = commands[command_index];

                source_type = None, dest_type = None;

                /* if expecting 1 arg */
                if(command.arg1_optional_types) {
                    token = strtok(NULL, COMMA_SEP);
                    if(token) {
                        if((source_type = get_arg_type(token, command.arg1_optional_types)) == None) {
                            line_error(INVALID_SOURCE_ARG, base_file_name, line_number, line);
                            error_flag = true;
                            continue;
                        }

                        if(!(source_type & command.arg1_optional_types)) {
                            line_error(INVALID_SOURCE_ARG, base_file_name, line_number, line);
                            error_flag = true;
                            continue;
                        } else {
                            command_length++;
                        }
                    } else {
                        line_error(TOO_FEW_ARGS, base_file_name, line_number, line);
                        error_flag = true;
                        continue;
                    }
                }

                /* if expecting 2 args, check if there is a second arg */
                if(command.arg2_optional_types) {
                    token = strtok(NULL, (source_type == None ? SPACE_SEP : COMMA_SEP));
                    if(token) {
                        /* ok lets check for Jump type */
                        if ((dest_type = get_arg_type(token, command.arg2_optional_types)) == None) {
                            line_error(INVALID_DEST_ARG, base_file_name, line_number, line);
                            error_flag = true;
                            continue;
                        }
                        if (!(dest_type & command.arg2_optional_types)) {
                            line_error(INVALID_DEST_ARG, base_file_name, line_number, line);
                            error_flag = true;
                            continue;
                        } else {
                            if(dest_type == Jump) {
                                /* get jump params and check the length of overall command */
                                command_length += getJumpParamsLength(token);
                            } else {
                                command_length++;
                            }
                        }
                    } else {
                        line_error(TOO_FEW_ARGS, base_file_name, line_number, line);
                        error_flag = true;
                        continue;
                    }

                }

                if(strtok(NULL, SPACE_SEP) != NULL) {
                    line_error(TOO_MANY_ARGS, base_file_name, line_number, line);
                    error_flag = true;
                    continue;
                }

                /* if both args are registers, decrease command length by 1 */
                if(source_type == Register && dest_type == Register)
                    command_length--;

                IC += command_length;
                /* printf("\t%lu command: %s length: %d\n", line_number, command.name, command_length); */
            } else {
                line_error(COMMAND_OR_DATA_INSTRUCTION_NOT_FOUND, base_file_name, line_number, line);
                error_flag = true;
                continue;
            }
        }
    }

    /* update DC */
    updateDC(IC, label_list, NULL);
    DC += IC;

    /* printf("IC: %d DC: %d\n", IC, DC); */

    if(DC-START_ADD > MAX_MEMORY_SIZE) {
        file_error(MEMORY_OVERFLOW, base_file_name);
        error_flag = true;
    }

    /* start second run */
    rewind(file);
    return second_run(IC, DC, label_list, extern_list, entry_list, error_flag, file ,base_file_name);
}
