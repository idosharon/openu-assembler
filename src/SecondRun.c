#include "SecondRun.h"


int second_run(int IC, int DC,
               node_t* label_list,
               node_t* extern_list,
               node_t* entry_list,
               bool error_flag,
               FILE* file,
               char* base_file_name) {

    /* create memory images */
    word *code_image = (word *) malloc((IC - START_ADD) * sizeof(word));
    word *memory_image = (word *) malloc((DC - IC) * sizeof(word));

    /* extern and entry show list */
    node_t *extern_show_list = NULL;
    node_t *entry_show_list = NULL;

    /* line buffer */
    char *line = (char *) malloc(MAX_LINE_SIZE * sizeof(char));
    size_t line_number = 0;
    size_t command_index = 0;

    /* token pointer */
    char *token;

    /* labels, extern and entry */
    char *current_label_name = NULL;
    label_t *current_label = NULL;
    label_t *current_entry = NULL;
    label_t *current_extern = NULL;

    /* encoded word */
    word* encoded_word = (word*) calloc(sizeof(word), 1);
    argument_t arg1 = EMPTY_ARGUMENT, arg2 = EMPTY_ARGUMENT;

    bool label_flag = false;

    /* current error code */
    ERROR error_code = NO_ERROR;

    int data, i = 0;

    /* command & data objects */
    binary_command binaryCommand = {0, 0, 0, 0, 0, 0};
    binary_data binaryData = {0};

    /* set IC & DC to 0 */
    IC = 0;
    DC = 0;

    /* read new line from file */
    while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
        /* increase line counter */
        line_number++;
        /* split line into tokens */
        token = strtok(strdup(line), SPACE_SEP);

        /* check if there is a label */
        if (strchr(token, LABEL_SEP)) {
            if (isValidLabel(token)) {
                current_label_name = strdup(token);
                current_label_name[strlen(current_label_name) - 1] = NULL_TERMINATOR;
                current_label = findLabel(current_label_name, label_list, extern_list, NULL);
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
                continue;
            }
        }
        /* check all symbol types */
        if (IS_DATA_SYMBOL(token)) {

            /* if label is type .entry, add to entry show list */
            if (label_flag) {
                if (findLabel(current_label_name, entry_list, NULL) != NULL)
                    entry_show_list = addLabelNode(entry_show_list, current_label_name, DC, Data);
                label_flag = false;
            }
            /* check for .data symbol */
            if (isStrEqual(token, DATA_SYMBOL)) {
                /* encode all data */
                while ((token = strtok(NULL, COMMA_SEP)) != NULL) {
                    if (is_number(token)) {
                        /* TODO: calculate if number if out of range */
                        /* TODO: check the sign of the number (+-) and calculate if negative */
                        binaryData.data = atoi(token);
                        memory_image[DC].data = binaryData;
                        DC++;
                    } else {
                        continue;
                    }
                }
            } else if (isStrEqual(token, STRING_SYMBOL)) {
                token = strtok(NULL, SPACE_SEP);
                if (token == NULL) {
                    continue;
                }
                if (token[0] != STRING_QUOTE || token[strlen(token) - 1] != STRING_QUOTE) {
                    continue;
                }
                token[strlen(token) - 1] = NULL_TERMINATOR;
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
        } else if (IS_EXTERN_SYMBOL(token) || IS_ENTRY_SYMBOL(token)) {
            continue;
        } else {
            /* get current command index */
            if ((command_index = find_command(token)) != ERROR_CODE) {
                /* if label is type .entry, add to entry show list */
                if (label_flag) {
                    if (findLabel(current_label_name, entry_list, NULL) != NULL)
                        entry_show_list = addLabelNode(entry_show_list, current_label_name, IC, Code);
                    label_flag = false;
                }

                /* check command type (group) */
                int offset = 0;
                command_t command = commands[command_index];

                bool is_jump = (!command.arg1_optional_types) && (command.arg2_optional_types & Jump);

                arg_type source_type = None, dest_type = None;

                /* reset args */
                resetArg(arg1);
                resetArg(arg2);

                /* encoding type always absolute */
                binaryCommand.encoding_type = Absolute;

                /* set params type to default None */
                binaryCommand.first_par_type = None;
                binaryCommand.second_par_type = None;

                /* set args type to default None */
                binaryCommand.dest_type = None;
                binaryCommand.src_type = None;

                binaryCommand.opcode = command_index;

                /* if expecting first arg */
                if (command.arg1_optional_types) {
                    /* continue to first arg */
                    offset++;
                    token = strtok(NULL, COMMA_SEP);

                    if (token) {
                        if ((source_type = get_arg_type(token, Immediate | Direct | Register)) == None) {
                            continue;
                        }
                        /* if arg type is in the optional types for command continue */
                        if (source_type & command.arg1_optional_types) {
                            /* if arg type is valid, encode it */
                            binaryCommand.src_type = encodeArgumentType(source_type);

                            /* save current token and type in argument 1 */
                            arg1.value = token;
                            arg1.type = source_type;

                            /* get encoded word of argument */
                            if ((error_code = encodeArgumentToWord(encoded_word, arg1, arg2, label_list, extern_list))) {
                                line_error(error_code, base_file_name, line_number);
                                error_flag = true;
                            }

                            /* TODO: fix this */
                            if (encoded_word->param.encoding_type == External) {
                                /* add to external show list */
                                extern_show_list = addLabelNode(extern_show_list, token, IC + offset, Code);
                            }

                            /* add encoded word to code image */
                            code_image[IC + offset] = *encoded_word;
                        }
                    } else {
                        /* if expecting 1 arg and not found raise error */
                        continue;
                    }
                }




                if (strtok(NULL, SPACE_SEP) != NULL) {
                    continue;
                }

                /* encode command into code image */
                code_image[IC].command = binaryCommand;
                IC += offset + 1;


                printf("\t%lu command: %s length: %d\n", line_number, command.name, offset + 1);
            } else {
                line_error(COMMAND_NOT_FOUND, base_file_name, line_number);
                continue;
            }
        }
    }

    /* if all good  create entry and external files from lists */
    if (!error_flag) {
        printf("No errors found, creating output files: %s\n", base_file_name);

        createObjFile(IC, code_image, DC, memory_image, base_file_name);

        /* add the IC address and START ADDRESS to all entry and extern labels */
        IC += START_ADD;
        updateIC(START_ADD, entry_show_list, extern_show_list, NULL);
        updateDC(IC, entry_show_list, extern_show_list, NULL);

        if (entry_show_list != NULL) {
            createEntryFile(entry_show_list, base_file_name);
        }
        if (extern_show_list != NULL) {
            createExternFile(extern_show_list, base_file_name);
        }

    }
    return error_flag;
}

ERROR encodeArgument(word* code_image, argument_t current_arg, node_t* label_list, node_t* extern_list, node_t** extern_show_list) {
    if (command.arg2_optional_types) {
        /* continue to second arg */
        offset++;
        token = strtok(NULL, is_jump ? LINE_BREAK : COMMA_SEP);

        if (token) {
            /* ok lets check for Jump type */
            if ((dest_type = get_arg_type(token, Immediate | Jump | Direct | Register)) == None) {
                continue;
            }
            if (dest_type & command.arg2_optional_types) {
                binaryCommand.dest_type = encodeArgumentType(dest_type);

                if(dest_type == Jump) {



                } else {
                    arg2.value = token;
                    arg2.type = dest_type;

                    if(source_type == Register && dest_type == Register) {
                        offset--;
                    }

                    /* add encoded word to code image */
                    if ((error_code = encodeArgumentToWord(code_image + IC + offset, arg2, arg1, label_list, extern_list))) {
                        line_error(error_code, base_file_name, line_number);
                        error_flag = true;
                    }

                    if (code_image[IC + offset].param.encoding_type == External) {
                        /* add to external show list */
                        extern_show_list = addLabelNode(extern_show_list, token, IC + offset, Code);
                    }
                }
            }
        } else {
            continue;
        }
    }
}

ERROR encodeArgumentInImage(word* code_image, argument_t current_arg, argument_t prev_arg,
                           node_t* label_list, node_t* extern_list) {
    binary_param binaryParam = {0};
    binary_two_registers binaryTwoRegisters = {0};

    label_t* current_label;

    switch (current_arg.type) {
        case Immediate:
            current_arg.value++;
            if (is_number(current_arg.value)) {
                /* TODO: check for negative number */
                binaryParam.data = atoi(current_arg.value);
                binaryParam.encoding_type = Absolute;
                code_image->param = binaryParam;
            }
            break;
        case Direct:
            current_label = findLabel(current_arg.value, label_list, extern_list, NULL);
            if (current_label == NULL) {
                return UNDEFINED_LABEL;
            }

            if (current_label->type == Extern) {
                binaryParam.encoding_type = External;
                binaryParam.data = 0;
            } else {
                binaryParam.encoding_type = Relocatable;
                binaryParam.data = current_label->place;
            }
            code_image->param = binaryParam;
            break;
        case Register:
            binaryTwoRegisters.encoding_type = Absolute;

            binaryTwoRegisters.src_register = find_register(current_arg.value);

            if (prev_arg.type == Register) {
                binaryTwoRegisters.dest_register = binaryTwoRegisters.src_register;
                binaryTwoRegisters.src_register = find_register(prev_arg.value);
            }

            code_image->two_registers = binaryTwoRegisters;
            break;
        case Jump:


            break;
        case None:
            break;
    }

    return NO_ERROR;
}

ERROR encodeJumpCommandToWord(word* code_image, argument_t jmp_arg, node_t* label_list, node_t* extern_list) {
    binary_command JmpBinaryCommand = {0};



    return NO_ERROR;
}


void createEntryFile(node_t* entry_show_list, char* base_file_name) {
    char* entry_file_name;
    FILE* output_entry_file;
    label_t* current_entry;

    if(!base_file_name) return;

    if (!entry_show_list) return;

    if(!(entry_file_name = getFileName(base_file_name, ENT_FILE_EXTENSION))) {
        return;
    }

    output_entry_file = fopen(entry_file_name, FILE_WRITE_MODE);

    while(entry_show_list) {
        current_entry = entry_show_list->data;
        fprintf(output_entry_file, "%s\t%lu\n",current_entry->name , current_entry->place);
        entry_show_list = (node_t *) entry_show_list->next;
    }

    fclose(output_entry_file);
}


void createExternFile(node_t* extern_show_list, char* base_file_name) {
    char* extern_file_name;
    FILE* output_extern_file;
    label_t* current_extern;

    if(!base_file_name) return;

    if (!extern_show_list) return;

    if(!(extern_file_name = getFileName(base_file_name, EXT_FILE_EXTENSION))) {
        return;
    }

    output_extern_file = fopen(extern_file_name, FILE_WRITE_MODE);

    while(extern_show_list) {
        current_extern = extern_show_list->data;
        fprintf(output_extern_file, "%s\t%lu\n",current_extern->name , current_extern->place);
        extern_show_list = (node_t *) extern_show_list->next;
    }

    fclose(output_extern_file);
}

void createObjFile(int IC, word* code_image, int DC, word* memory_image, char* base_file_name) {
    char* obj_file_name;
    FILE* output_obj_file;
    int i = 0, j = 0;
    size_t current_word;

    if(!base_file_name) return;

    if(!(obj_file_name = getFileName(base_file_name, OBJ_FILE_EXTENSION))) {
        return;
    }

    output_obj_file = fopen(obj_file_name, FILE_WRITE_MODE);

    fprintf(output_obj_file, "object file for: %s%s\n", base_file_name, ASM_FILE_EXTENSION);
    fprintf(output_obj_file, "Base 10 address\tBase 2 code\n");
    fprintf(output_obj_file, "\t%d\t%d\n", IC, DC);

    fprintf(output_obj_file, "code image: \n");

    /* write code image */
    /* TODO: move to function */
    for (; i < IC; ++i) {
        current_word = code_image[i].data.data;
        fprintf(output_obj_file, "%04d\t", i + START_ADD);
        /* print data in binary 1 and 0 */
        writeBinToFile(current_word, WORD_SIZE, output_obj_file);
        fputc('\n', output_obj_file);
    }

    fprintf(output_obj_file, "memory image: \n");

    /* write memory image */
    for (i = 0; i < DC; ++i) {
        current_word = memory_image[i].data.data;
        fprintf(output_obj_file, "%04d\t", i + START_ADD + IC);
        /* print data in binary 1 and 0 */
        writeBinToFile(current_word, WORD_SIZE, output_obj_file);
        fputc('\n', output_obj_file);
    }
}

