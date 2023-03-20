#include "SecondRun.h"


int second_run(int IC, int DC, node_t* label_list, node_t* extern_list, node_t* entry_list, bool error_flag, FILE* file,char* base_file_name) {

    word* code_image = (word*)malloc((IC-START_ADD) * sizeof(word));
    word* memory_image = (word*)malloc((DC-IC) * sizeof(word));

    node_t* extern_show_list = NULL;
    node_t* entry_show_list = NULL;

    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof (char));
    size_t line_number = 0;
    size_t command_index = 0;
    char* token;
    char* current_label_name = NULL;

    label_t* current_label = NULL;
    label_t* current_entry = NULL;
    label_t* current_extern = NULL;

    bool label_flag = false;

    binary_command binaryCommand = {0,0,0,0,0,0};
    binary_data binaryData = {0};
    binary_param binaryFirstParam = {0,0};
    binary_param binarySecondParam = {0,0};
    binary_two_registers binaryTwoRegisters = {0,0,0};

    DC = 0;
    IC = 0;

    /* read new line from file */
    while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
        /* increase line counter */
        line_number++;
        /* split line into tokens */
        token = strtok(strdup(line), SPACE_SEP);

        /* check if there is a label */
        if(strchr(token, LABEL_SEP)) {
            if (isValidLabel(token)) {
                current_label_name = strdup(token);
                current_label_name[strlen(current_label_name)-1] = NULL_TERMINATOR;
                current_label = findLabel(current_label_name, label_list, extern_list);
                if (current_label == NULL) {
                    line_error(UNDEFINED_LABEL, base_file_name, line_number);
                    error_flag = true;
                    continue;
                }
                if (current_label->type == Extern) {
                    line_error(CONFLICT_LOCAL_EXTERNAL_LABELS, base_file_name, line_number);
                    error_flag = true;
                    continue;
                }
                label_flag = true;
                token = strtok(NULL, SPACE_SEP);
            } else {
                error_flag = true;
                continue;
            }
        }
        /* check all symbol types */
        if (IS_DATA_SYMBOL(token)) {

            if (label_flag) {
                if (current_label->type == Entry) {
                    entry_show_list = addLabelNode(entry_show_list, current_label->name, DC, Data);
                }
                label_flag = false;
            }

            /* check for .data symbol */
            if (isStrEqual(token, DATA_SYMBOL)) {
                /* calculate data length */
                while((token = strtok(NULL, COMMA_SEP)) != NULL) {
                    if (is_number(token)) {
                        /* TODO: calculate if number if out of range */
                        /* TODO: check the sign of the number (+-) and calculate if negative */
                        binaryData.data = atoi(token);
                        memory_image[DC].data = binaryData;
                        DC++;
                    } else {
                        error_flag = true;
                        continue;
                    }
                }
            }
            else if (isStrEqual(token, STRING_SYMBOL)) {
                token = strtok(NULL,SPACE_SEP);
                if (token == NULL) {
                    error_flag = true;
                    continue;
                }
                if (token[0] != STRING_QUOTE || token[strlen(token)-1] != STRING_QUOTE) {
                    error_flag = true;
                    continue;
                }
                token[strlen(token)-1] = NULL_TERMINATOR;
                token++;
                while (token[0] != NULL_TERMINATOR) {
                    binaryData.data = token[0];
                    memory_image[DC].data = binaryData;
                    DC++;
                    token++;
                }
                binaryData.data = 0;
                memory_image[DC].data = binaryData;
                DC++;
            }
        }
        else if (IS_EXTERN_SYMBOL(token) || IS_ENTRY_SYMBOL(token)) {
            /* case: extern symbol */
            if(IS_EXTERN_SYMBOL(token)) {
                /* get external label */
                token = strtok(NULL,SPACE_SEP);

                /* if not found raise error */
                if (token == NULL) {
                    error_flag = true;
                    continue;
                }
                /* if more than one found raise error */
                if(strtok(NULL,SPACE_SEP) != NULL) {
                    error_flag = true;
                    continue;
                }
                /* if the label is not valid raise error */
                if (!isValidLabel(token)) {
                    error_flag = true;
                    continue;
                }
            } else {

                /* get entry label */
                token = strtok(NULL,SPACE_SEP);

                /* if no token found raise error */
                if (token == NULL) {
                    error_flag = true;
                    continue;
                }
                /* if more than one found raise error */
                if(strtok(NULL,SPACE_SEP) != NULL) {
                    error_flag = true;
                    continue;
                }
                /* if label is not valid raise error */
                if (!isValidLabel(token)) {
                    error_flag = true;
                    continue;
                }
            }
        }
        else {

            /* get current command index */
            if ((command_index = find_command(token)) != -1) {

                /* check command type (group) */
                int command_length = 1;
                command_t command = commands[command_index];

                bool is_jump = (!command.arg1) && (command.arg2 & Jump);

                arg_type source_type = None, dest_type = None;

                /* encoding type always absolute */
                binaryCommand.encoding_type = Absolute;

                /* if expecting first arg */
                if(command.arg1) {
                    token = strtok(NULL, COMMA_SEP);
                    if(token) {
                        if((source_type = get_arg_type(token, Immediate | Direct | Register)) == None) {
                            error_flag = true;
                            continue;
                        }
                        /* if arg type is not valid raise error */
                        if(!(source_type & command.arg1)) {
                            error_flag = true;
                            continue;
                        }
                        /* if arg type is valid, encode it */
                        binaryCommand.src_type = log2(source_type);

                        command_length++;
                    } else {
                        /* if expecting 1 arg and not found raise error */
                        error_flag = true;
                        continue;
                    }
                }

                /* if expecting 2 args, check if there is a second arg */
                if(command.arg2) {
                    token = strtok(NULL, is_jump ? LINE_BREAK : COMMA_SEP);
                    if(token) {
                        /* ok lets check for Jump type */
                        if ((dest_type = get_arg_type(token, Immediate | Jump | Direct | Register)) == None) {
                            error_flag = true;
                            continue;
                        }
                        if (!(dest_type & command.arg2)) {
                            error_flag = true;
                            continue;
                        }

                        binaryCommand.opcode = command_index;

                        /* TODO: Depth analysis of token to get exact length */
                        binaryCommand.dest_type = log2(dest_type);

                        if(dest_type == Jump) {
                            /* get jump params and check the length of overall command */
                            int first_param_type, second_param_type;
                            first_param_type = getJumpParamType(token, 1);
                            second_param_type = getJumpParamType(token, 2);
                            if (first_param_type == -1 || second_param_type == -1) {
                                error_flag = true;
                                continue;
                            }
                            binaryCommand.first_par_type = log2(first_param_type);
                            binaryCommand.second_par_type = log2(second_param_type);
                            command_length += 2;
                            if (first_param_type == Register && second_param_type == Register)
                                command_length--;

                        } else {
                            command_length++;
                            binaryCommand.first_par_type = None;
                            binaryCommand.second_par_type = None;
                        }
                    } else {
                        /* if expecting 2 args and not found raise error */
                        error_flag = true;
                        continue;
                    }
                }

                if((token = strtok(NULL, SPACE_SEP)) != NULL) {
                    error_flag = true;
                    continue;
                }
                code_image[IC].command = binaryCommand;
                /*code_image[++IC].param = 0;*/
                IC += command_length;
                printf("\t%d command: %s length: %d\n", line_number, command.name, command_length);
            } else {
                line_error(COMMAND_NOT_FOUND, base_file_name, line_number);
                continue;
            }
        }
    }

    /* print all command image and data image */
    printf("IC: %d DC: %d\n", IC, DC);
    int i;
    for (i = 0; i < DC; ++i)
        printf("DC: %d data: %d\n", i, memory_image[i].data.data);

    /* if all good  create entry and external files from lists */
    if(!error_flag) {
        printf("No errors found, creating output files: %s\n", base_file_name);
        if (entry_list != NULL) {
            /* create_entry_file(entry_list, base_file_name); */
        }
        if (extern_list != NULL) {
            /* create_extern_file(extern_list, base_file_name); */
        }
    }
    return error_flag;
}


