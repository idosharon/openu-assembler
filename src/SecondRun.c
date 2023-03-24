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
    binary_param* binaryFirstParam = (binary_param*)calloc(sizeof(binary_param), 1);
    binary_param* binarySecondParam = (binary_param*)calloc(sizeof(binary_param), 1);

    binarySecondParam->data = 0;
    binarySecondParam->encoding_type = None;
    binaryFirstParam->data = 0;
    binaryFirstParam->encoding_type = None;

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
                        if ((source_type = get_arg_type(token, command.arg1_optional_types)) == None) {
                            continue;
                        }
                        /* if arg type is valid, encode it */
                        binaryCommand.src_type = encodeArgumentType(source_type);

                        /* save current token and type in argument 1 */
                        arg1.value = token;
                        arg1.type = source_type;

                        /* get encoded word of argument */
                        if ((error_code = encodeArgumentToWord(token, (word **) &binaryFirstParam, -1, source_type,
                                                               label_list, extern_list, &extern_show_list, IC+offset))) {
                            line_error(error_code, base_file_name, line_number);
                            error_flag = true;
                        }

                        /* encode word */
                        code_image[IC+offset].param = *binaryFirstParam;


                    } else {
                        /* if expecting 1 arg and not found raise error */
                        continue;
                    }
                }

                /* if expecting 2 args, check if there is a second arg */
                if (command.arg2_optional_types) {
                    /* continue to second arg */
                    offset++;
                    token = strtok(NULL,SPACE_SEP);
                    arg2.value = strdup(token);
                    token = strtok(token, is_jump ? JMP_OPEN_BRACKET : COMMA_SEP);
                    if (token) {
                        if ((dest_type = get_arg_type(token, command.arg2_optional_types)) == None) {
                            continue;
                        }

                        is_jump = (getJumpParamsLength(arg2.value) != -1);

                        /* if arg type is valid, encode it */
                        if (is_jump)
                            binaryCommand.dest_type = encodeArgumentType(Jump);
                        else
                            binaryCommand.dest_type = encodeArgumentType(dest_type);

                        /* save current token and type in argument 2 */
                        arg2.type = dest_type;

                        /* get encoded word of argument */
                        if ((error_code = encodeArgumentToWord(token, (word **) &binarySecondParam, (source_type == Register ? find_register(arg1.value) : -1), dest_type,
                                                               label_list, extern_list, &extern_show_list, IC+offset))) {
                            line_error(error_code, base_file_name, line_number);
                            error_flag = true;
                        }

                        /* encode word */
                        if (source_type == Register && dest_type == Register) {
                            offset--;
                        }

                        code_image[IC+offset].param = *binarySecondParam;

                        if (is_jump) {
                            binary_param* binaryJumpFirstParam = (binary_param*)calloc(sizeof(binary_param), 1);
                            binary_param* binaryJumpSecondParam = (binary_param*)calloc(sizeof(binary_param), 1);

                            arg_type jmpFirstParmType = None, jmpSecondParmType = None;

                            offset++;
                            token = strtok(arg2.value, JMP_OPEN_BRACKET);
                            token = strtok(NULL, COMMA_SEP);
                            if (token) {
                                if ((jmpFirstParmType = get_arg_type(token, Immediate | Direct | Register)) == None) {
                                    continue;
                                }
                                /* if arg type is valid, encode it */
                                binaryCommand.second_par_type = encodeArgumentType(jmpFirstParmType);

                                /* save current token and type in argument 1 */
                                arg1.value = token;
                                arg1.type = jmpFirstParmType;

                                /* get encoded word of argument */
                                if ((error_code = encodeArgumentToWord(token, (word **) &binaryJumpFirstParam, -1,
                                                                       jmpFirstParmType,
                                                                       label_list, extern_list, &extern_show_list,
                                                                       IC + offset))) {
                                    line_error(error_code, base_file_name, line_number);
                                    error_flag = true;
                                }

                                /* encode word */
                                code_image[IC + offset].param = *binaryJumpFirstParam;
                            }
                            else {
                                continue;
                            }
                            offset++;
                            token = strtok(NULL, JMP_CLOSE_BRACKET);
                            if (token) {
                                if ((jmpSecondParmType = get_arg_type(token, Immediate | Direct | Register)) == None) {
                                    continue;
                                }
                                /* if arg type is valid, encode it */
                                binaryCommand.first_par_type = encodeArgumentType(jmpSecondParmType);

                                /* get encoded word of argument */
                                if ((error_code = encodeArgumentToWord(token, (word **) &binaryJumpSecondParam, (jmpFirstParmType == Register ? find_register(arg1.value) : 0), jmpSecondParmType,
                                                                       label_list, extern_list, &extern_show_list, IC+offset))) {
                                    line_error(error_code, base_file_name, line_number);
                                    error_flag = true;
                                }

                                /* encode word */
                                if (jmpFirstParmType == Register && jmpSecondParmType == Register) {
                                    offset--;
                                }

                                /* encode word */
                                code_image[IC+offset].param = *binaryJumpSecondParam;
                            }
                            else {
                                continue;
                            }
                        }
                    } else {
                        continue;
                    }
                }


                if (strtok(NULL, SPACE_SEP) != NULL) {
                    continue;
                }

                /* encode command into code image */
                code_image[IC].command = binaryCommand;
                IC += offset + 1;


               /* printf("\t%lu command: %s length: %d\n", line_number, command.name, offset + 1); */
            } else {
                line_error(COMMAND_NOT_FOUND, base_file_name, line_number);
                continue;
            }
        }
    }

    error_flag = error_flag | checkForUndefinedEntries(entry_list,entry_show_list,base_file_name);

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

    /* TODO: free all allocated memory */

    info_file("Finished second run", base_file_name);

    return error_flag;
}

ERROR encodeArgumentToWord(char* token, word** binArg, int prev_register, arg_type optional_types,
                    node_t* label_list, node_t* extern_list, node_t** extern_show_list, int IC) {
    arg_type current_type;
    label_t* current_label;

    if ((current_type = get_arg_type(token, optional_types)) == None) {
        return INVALID_ARG_TYPE;
    }

    switch (current_type) {
        case Immediate:
            token++;
            (*binArg)->param.encoding_type = Absolute;
            (*binArg)->param.data = atoi(token);
            break;
        case Direct:
            (*binArg)->param.encoding_type = Relocatable;
            if ((current_label = findLabel(token, label_list, extern_list)) == NULL) {
                return UNDEFINED_LABEL;
            }
            if (current_label->type == Extern) {
                (*binArg)->param.encoding_type = External;
                (*binArg)->param.data = 0;
                *extern_show_list = addLabelNode(*extern_show_list, token, IC, Code);
            }
            else
                (*binArg)->param.data = current_label->place;
            break;
        case Register:
            (*binArg)->param.encoding_type = Absolute;
            if (prev_register == -1) {
                (*binArg)->two_registers.encoding_type = Absolute;
                (*binArg)->two_registers.src_register = find_register(token);
                (*binArg)->two_registers.dest_register = 0;
            } else {
                (*binArg)->two_registers.encoding_type = Absolute;
                (*binArg)->two_registers.dest_register = find_register(token);
                (*binArg)->two_registers.src_register = prev_register;
            }
            break;
    }
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

bool checkForUndefinedEntries(node_t* entry_list, node_t* entry_show_list, char* base_file_name) {

    bool error_flag = false;

    while(entry_list != NULL) {
        if (!findLabelInList(((label_t*) entry_list->data)->name, entry_show_list)) {
            line_error(UNDEFINED_LABEL, base_file_name, ((label_t*) entry_list->data)->place);
            error_flag = true;
        }
        entry_list = (node_t*) entry_list->next;
    }
    return error_flag;
}

