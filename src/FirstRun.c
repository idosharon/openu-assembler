#include "FirstRun.h"

int firstRun(FILE* file, char* base_file_name) {
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof (char));
    size_t line_number = 0;
    size_t command_index = 0;
    char* token;

    /* create list for labels */
    node_t* label_list = NULL;
    node_t* future_label_list = NULL;

    char* current_label = NULL;
    bool label_flag = false;

    /* error flag */
    bool error_flag = false;

    /* memory image flag */
    bool memory_image_flag = false;

    /* create list for external and entry labels */
    node_t* extern_list = NULL;
    node_t* entry_list = NULL;

    /* init IC and DC to START ADDRESS = 100 */
    size_t IC = START_ADD;
    size_t DC = START_ADD;


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
                /* check if label exists in future labels */
                if (findLabel(current_label, label_list)) {
                    line_error(MULTIPLE_LABEL_DEFINITIONS, base_file_name, line_number);
                    error_flag = true;
                    continue;
                }

                /* delete label from future labels list */
                deleteLabel(token, &future_label_list);

                label_flag = true;
                current_label = token;
                current_label[strlen(current_label)-1] = '\0';
                token = strtok(NULL, SPACE_SEP);
            } else {
                /* label is not valid */
                line_error(LABEL_SYNTAX_ERROR, base_file_name, line_number);
                error_flag = true;
                continue;
            }
        }

        /* check all symbol types */
        if (IS_DATA_SYMBOL(token)) {
            memory_image_flag = true;
            /* check if label is already defined */
            if (label_flag) {
                label_list = addLabelNode(label_list, current_label, DC, Data);
            }

            /* check for .data symbol */
            if (isStrEqual(token, DATA_SYMBOL)) {
                /* calculate data length */
                while((token = strtok(NULL, COMMA_SEP)) != NULL) {
                    if (is_number(token)) {
                        /* TODO: calculate if number if out of range and number of words needed for it */
                        DC++;
                    } else {
                        line_error(DATA_SYNTAX_ERROR, base_file_name,line_number);
                        error_flag = true;
                        continue;
                    }
                }
            }
            else if (isStrEqual(token, STRING_SYMBOL)) {
                token = strtok(NULL,SPACE_SEP);
                DC += strlen(token)-1;
            }
        }
        else if (IS_EXTERN_SYMBOL(token) || IS_ENTRY_SYMBOL(token)) {
            /* get external/entry label */
            token = strtok(NULL,SPACE_SEP);

            /* case: extern symbol */
            if(IS_EXTERN_SYMBOL(token)) {
                /* if not found raise error */
                if (token == NULL) {
                    line_error(EXTERN_MISSING_ARGUMENT,base_file_name,line_number);
                    error_flag = true;
                    continue;
                }
                /* if more than one found raise error */
                if(strtok(NULL,SPACE_SEP) != NULL) {
                    line_error(EXTERN_TOO_MANY_ARGUMENTS, base_file_name,line_number);
                    error_flag = true;
                    continue;
                }
                /* if the label is not valid raise error */
                if (!isValidLabel(token)) {
                    line_error(LABEL_SYNTAX_ERROR,base_file_name,line_number);
                    error_flag = true;
                    continue;
                }

                /* add to external list */
                extern_list = addLabelNode(extern_list, token, 0, Extern);
            } else {
                /* if no token found raise error */
                if (token == NULL) {
                    line_error(ENTRY_MISSING_ARGUMENT,base_file_name,line_number);
                    continue;
                }
                /* if more than one found raise error */
                if(strtok(NULL,SPACE_SEP) != NULL) {
                    line_error(ENTRY_TOO_MANY_ARGUMENTS, base_file_name,line_number);
                    error_flag = true;
                    continue;
                }
                /* if label is not valid raise error */
                if (!isValidLabel(token)) {
                    line_error(LABEL_SYNTAX_ERROR,base_file_name,line_number);
                    continue;
                }
                /* add to entry list */
                entry_list = addLabelNode(entry_list, token, 0, Entry);
            }

            /* delete the label from future list if found */
            deleteLabel(token, &future_label_list);

        }
        else {
            /* if in memory image return code after data error */
            if(memory_image_flag) {
                line_error(CODE_AFTER_DATA, base_file_name, line_number);
                error_flag = true;
                continue;
            }

            /* add label if needed */
            if (label_flag) {
                label_list = addLabelNode(label_list, current_label, IC, Code);
            }

            /* get current command index */
            if ((command_index = find_command(token)) != -1) {

                /* check command type (group) */
                short command_length = 1;
                int args_counter = 0;
                command_t command = commands[command_index];

                bool is_jump = (!command.arg1) && (command.arg2 & Jump);

                arg_type source_type = None, dest_type = None;

                /* if expecting 1 arg */
                if(command.arg1) {
                    token = strtok(NULL, COMMA_SEP);
                    if(token) {
                        if((source_type = get_arg_type(token, Immediate | Direct | Register)) == -1) {
                            line_error(PRESERVED_KEYWORD, base_file_name, line_number);
                            continue;
                        }
                        if(!(source_type & command.arg1))
                            line_error(INVALID_ARG_TYPE, base_file_name, line_number);

                        /* if first arg is a label, add it to label list */
                        if(source_type == Direct) {
                            if(findLabel(token, label_list) == NULL) {
                                addLabelNode(future_label_list, token, 0, Code);
                            }
                        }

                        command_length++;



                    } else {
                        line_error(TOO_FEW_ARGS, base_file_name, line_number);
                        continue;
                    }
                }

                /* if expecting 2 args, check if there is a second arg */
                if(command.arg2) {
                    if(is_jump) {
                        token = strtok(NULL, OPEN_BRACKET);
                        if(!isValidLabel(token)) {
                            line_error(LABEL_SYNTAX_ERROR, base_file_name, line_number);
                            continue;
                        }


                    } else {
                        token = strtok(NULL, COMMA_SEP);

                        if(token) {
                            if ((dest_type = get_arg_type(token, Immediate | Jump | Direct | Register)) == -1) {
                                line_error(PRESERVED_KEYWORD, base_file_name, line_number);
                                continue;
                            }
                            if (!(dest_type & command.arg2))
                                line_error(INVALID_ARG_TYPE, base_file_name, line_number);
                            command_length++;
                        } else {
                            line_error(TOO_FEW_ARGS, base_file_name, line_number);
                            continue;
                        }
                    }


                }

                if((token = strtok(NULL, SPACE_SEP)) != NULL) {
                    line_error(TOO_MANY_ARGS, base_file_name, line_number);
                    continue;
                }

                /* if both args are registers, decrease command length by 1 */
                if(source_type == Register && dest_type == Register) {

                    command_length--;
                }

                IC += command_length;
                /*binary_str = getBinaryCommand(token, base_file_name, line_number);
                if (binary_str == NULL) {
                    continue;
                }
                else {
                    IC += get_command_length(binary_str);
                }*/
            } else {
                line_error(COMMAND_NOT_FOUND, base_file_name, line_number);
                continue;
            }
        }
    }

    /* if all good  create entry and external files from lists */
    /*if (entry_list != NULL) {
        create_entry_file(entry_list, base_file_name);
    }
    if (extern_list != NULL) {
        create_extern_file(extern_list, base_file_name);
    }*/


    info_file("Finished first run", base_file_name);
    return error_flag;
}



label_t* findLabel(char* name, node_t* head) {
    label_t* label;
    while(head != NULL) {
        if(isStrEqual((label = (label_t*) head->data)->name, name)) {
            return label;
        }
        head = (node_t*) head->next;
    }
    return NULL;
}

bool deleteLabel(char* name, node_t** head) {
    node_t* current = *head;
    node_t* prev = NULL;
    label_t* label;
    while(current != NULL) {
        if(isStrEqual((label = (label_t*) current->data)->name, name)) {
            if(prev == NULL) {
                *head = (node_t*) current->next;
            } else {
                prev->next = current->next;
            }
            free(label->name);
            free(label);
            free(current);
            return true;
        }
        prev = current;
        current = (node_t*) current->next;
    }
    return false;
}

bool isValidLabel(char* label_name) {
    /* check if label name is valid - only contains alphabetic and numbers */
    if (!isValidLabelFormat(label_name)
            || find_register(label_name) != -1
            || find_command(label_name) != -1) {
        return false;
    }
    return true;
}

node_t* addLabelNode(node_t* head, char* name, size_t place, label_type labelType) {
    /* set current label to the new label */
    label_t* new_label = (label_t*) (malloc(sizeof (label_t)));
    node_t* new_label_node = (node_t*) (malloc(sizeof (node_t)));

    /* set label name and allocate its data */
    new_label->name = name;
    new_label->place = place;
    new_label->type = labelType;

    /* create new label node and append it to the start of list */
    new_label_node->data = new_label;
    new_label_node->next = (struct node_t *) head;
    return new_label_node;
}


