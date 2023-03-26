/* File: SecondRun.c
 * Type: Source file
 * Description: Second run module, handles the second run of the assembler.
 *              if no errors found also in first run - creates the object, entry and extern files.
 *              if errors found in first or second run - returns error flag and reports all errors to stdout.
 * Authors: Ido Sharon (215774142)
 *         Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#include "SecondRun.h"

/* Function: second_run
 * Description: second run of the assembler, creates the object, entry and extern files and reports errors that couldn't find in first run to stdout.
 * Input: IC - instruction counter (number of words in code image),
 *        DC - data counter (number of words in memory image),
 *        label_list - list of all labels (including data labels),
 *        extern_list - list of all extern labels (all labels that are used as operands in .extern),
 *        entry_list - list of all entry labels (all labels that are used as operands in .entry),
 *        error_flag - flag indicating if errors were found in first run (error flag from first run),
 *        file - pointer to file to assemble (file pointer to .am (after pre-assembler) file),
 *        base_file_name - base file name (excluding extension, used for output file name with new .o, .ent, .ext extensions)
 * Output: 0 if no errors found, 1 if errors found
 */
int second_run(size_t IC, size_t DC,
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

    /* line related variables */
    char *line = (char *) malloc(MAX_LINE_SIZE * sizeof(char));
    size_t line_number = 0;

    /* token pointer */
    char *token;

    /* labels, extern and entry */
    char *current_label_name = NULL;
    label_t *current_label = NULL;
    bool label_flag;

    /* current error code */
    ERROR error_code;

    /* command related variables */
    int command_offset;
    command_t current_command;
    size_t command_index = 0;
    arg_type source_type = None, dest_type = None;

    /* .data/.string related variables */
    int current_data_value;
    /* pointers to start and finish of string in .string*/
    char *first_quote, *last_quote;

    /* current_command & current_data_value objects */
    binary_command binaryCommand = {0, 0, 0, 0, 0, 0};
    binary_data binaryData = {0};
    binary_param* binaryFirstParam = (binary_param*) calloc(sizeof(binary_param), 1);
    binary_param* binarySecondParam = (binary_param*) calloc(sizeof(binary_param), 1);

    /* reset parameters */
    resetParam(binaryFirstParam);
    resetParam(binarySecondParam);

    /* set IC & DC to 0 */
    IC = 0;
    DC = 0;

    /* read new line from file */
    while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
        /* increase line counter */
        line_number++;

        /* reset label flag & current label pointer */
        label_flag = false;
        current_label = NULL;

        /* split line into tokens */
        token = strtok(strdup(line), SPACE_SEP);

        /* check if there is a label */
        if (strchr(token, LABEL_SEP)) {
            if (isValidLabel(token)) {
                current_label_name = strdup(token);
                current_label_name[strlen(current_label_name) - 1] = NULL_TERMINATOR;
                current_label = findLabel(current_label_name, extern_list, label_list, NULL);
                label_flag = true;
                token = strtok(NULL, SPACE_SEP);
                if (token == NULL) {
                    continue;
                }
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
            if (current_label != NULL && current_label->type == Extern) {
                line_error(CONFLICT_LOCAL_EXTERNAL_LABELS, base_file_name, line_number, line);
                error_flag = true;
                continue;
            }

            /* check for .current_data_value symbol */
            if (isStrEqual(token, DATA_SYMBOL)) {
                /* encode all current_data_value */
                while ((token = strtok(NULL, COMMA_SEP)) != NULL) {
                    if (is_number(token)) {
                        current_data_value = atoi(token);
                        if (current_data_value > MAX_DATA_VALUE || current_data_value < MIN_DATA_VALUE) {
                            line_warning(DATA_OUT_OF_RANGE, base_file_name, line_number);
                        }
                        binaryData.data = current_data_value;
                        memory_image[DC].data = binaryData;
                        DC++;
                    } else {
                        continue;
                    }
                }
            } else if (isStrEqual(token, STRING_SYMBOL)) {
                token = strtok(NULL, SPACE_SEP);
                if (token == NULL)
                    continue;
                first_quote = strchr(line, STRING_QUOTE);
                last_quote = strrchr(line, STRING_QUOTE);
                if (last_quote == NULL || first_quote == NULL || last_quote <= first_quote) {
                    continue;
                }
                if (strtok(last_quote+1,SPACE_SEP) != NULL)
                    continue;

                strncpy(token, first_quote, last_quote-first_quote);
                token[last_quote-first_quote+1] = NULL_TERMINATOR;
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
            /* get current current_command index */
            if ((command_index = find_command(token)) != ERROR_CODE) {
                /* if label is type .entry, add to entry show list */
                if (label_flag) {
                    if (findLabel(current_label_name, entry_list, NULL) != NULL)
                        entry_show_list = addLabelNode(entry_show_list, current_label_name, IC, Code);
                    label_flag = false;
                }
                if (current_label != NULL && current_label->type == Extern) {
                    line_error(CONFLICT_LOCAL_EXTERNAL_LABELS, base_file_name, line_number, line);
                    error_flag = true;
                    continue;
                }

                /* check current_command type (group) */
                command_offset = 0;
                current_command = commands[command_index];

                /* encoding type always absolute */
                binaryCommand.encoding_type = Absolute;

                /* set params type to default None */
                binaryCommand.first_par_type = None;
                binaryCommand.second_par_type = None;

                /* set args type to default None */
                binaryCommand.dest_type = None;
                binaryCommand.src_type = None;

                binaryCommand.opcode = command_index;

                source_type = None, dest_type = None;

                /* if expecting first arg */
                if (current_command.arg1_optional_types) {
                    /* continue to first arg */
                    command_offset++;
                    token = strtok(NULL, COMMA_SEP);

                    if (token) {
                        if ((source_type = get_arg_type(token, current_command.arg1_optional_types)) == None) {
                            continue;
                        }
                        /* if arg type is valid, encode it */
                        binaryCommand.src_type = encodeArgumentType(source_type);


                        /* get encoded word of argument */
                        if ((error_code = encodeArgumentToWord(token, source_type, (word **) &binaryFirstParam, -1, label_list, extern_list))) {
                            if (error_code == DATA_OUT_OF_RANGE)
                                line_warning(error_code, base_file_name, line_number);
                            else {
                                error_flag = true;
                                line_error(error_code, base_file_name, line_number, line);
                                continue;
                            }
                        }

                        if(binaryFirstParam->encoding_type == External) {
                            extern_show_list = addLabelNode(extern_show_list, token, IC + command_offset, Code);
                        }
                        /* encode word */
                        code_image[IC + command_offset].param = *binaryFirstParam;


                    } else {
                        /* if expecting 1 arg and not found raise error */
                        continue;
                    }
                }

                /* if expecting 2 args, check if there is a second arg */
                if (current_command.arg2_optional_types) {
                    /* continue to second arg */
                    command_offset++;
                    token = strtok(NULL, (source_type == None ? SPACE_SEP : COMMA_SEP));

                    if (token) {
                        if ((dest_type = get_arg_type(token, current_command.arg2_optional_types)) == None) {
                            continue;
                        }

                        /* if arg type is valid, encode it */
                        binaryCommand.dest_type = encodeArgumentType(dest_type);
                        if(dest_type == Jump) {
                            token = strtok(token, OPEN_BRACKET);
                        }

                        /* get encoded word of argument */
                        if ((error_code = encodeArgumentToWord(token, (dest_type == Jump ? Direct : dest_type), (word**) &binarySecondParam,
                                                               (source_type == Register ? ((binary_two_registers*) binaryFirstParam)->src_register : 0), label_list, extern_list))) {
                            if (error_code == DATA_OUT_OF_RANGE)
                                line_warning(error_code, base_file_name, line_number);
                            else {
                                error_flag = true;
                                line_error(error_code, base_file_name, line_number, line);
                                continue;
                            }
                        }

                        if(binarySecondParam->encoding_type == External) {
                            extern_show_list = addLabelNode(extern_show_list, token, IC + command_offset, Code);
                        }

                        /* encode word */
                        if (source_type == Register && dest_type == Register) {
                            command_offset--;
                        }

                        code_image[IC + command_offset].param = *binarySecondParam;

                        if (dest_type == Jump) {
                            binary_param* binaryJumpFirstParam = (binary_param*) calloc(sizeof(binary_param), 1);
                            binary_param* binaryJumpSecondParam = (binary_param*) calloc(sizeof(binary_param), 1);

                            arg_type jmpFirstParmType = None, jmpSecondParmType = None;

                            command_offset++;
                            token = strtok(NULL, COMMA_SEP);

                            if (token) {
                                if ((jmpFirstParmType = get_arg_type(token, Immediate | Direct | Register)) == None) {
                                    continue;
                                }
                                /* if arg type is valid, encode it */
                                binaryCommand.second_par_type = encodeArgumentType(jmpFirstParmType);

                                /* get encoded word of argument */
                                if ((error_code = encodeArgumentToWord(token, jmpFirstParmType, (word **) &binaryJumpFirstParam, -1, label_list, extern_list))) {
                                    if (error_code == DATA_OUT_OF_RANGE)
                                        line_warning(error_code, base_file_name, line_number);
                                    else {
                                        error_flag = true;
                                        line_error(error_code, base_file_name, line_number, line);
                                        continue;
                                    }
                                }

                                if(binaryJumpFirstParam->encoding_type == External) {
                                    extern_show_list = addLabelNode(extern_show_list, token, IC + command_offset, Code);
                                }

                                /* encode word */
                                code_image[IC + command_offset].param = *binaryJumpFirstParam;
                            }
                            else {
                                continue;
                            }

                            command_offset++;
                            token = strtok(NULL, JMP_CLOSE_BRACKET);

                            if (token) {
                                if ((jmpSecondParmType = get_arg_type(token, Immediate | Direct | Register)) == None) {
                                    continue;
                                }
                                /* if arg type is valid, encode it */
                                binaryCommand.first_par_type = encodeArgumentType(jmpSecondParmType);

                                /* get encoded word of argument */
                                if ((error_code = encodeArgumentToWord(token, jmpSecondParmType, (word **) &binaryJumpSecondParam,
                                                                       (jmpFirstParmType == Register ? ((binary_two_registers*) binaryJumpFirstParam)->src_register : 0), label_list, extern_list))) {
                                    if (error_code == DATA_OUT_OF_RANGE)
                                        line_warning(error_code, base_file_name, line_number);
                                    else {
                                        error_flag = true;
                                        line_error(error_code, base_file_name, line_number, line);
                                    }
                                }

                                if(binaryJumpSecondParam->encoding_type == External) {
                                    extern_show_list = addLabelNode(extern_show_list, token, IC + command_offset, Code);
                                }

                                /* encode word */
                                if (jmpFirstParmType == Register && jmpSecondParmType == Register) {
                                    command_offset--;
                                }

                                /* encode word */
                                code_image[IC + command_offset].param = *binaryJumpSecondParam;
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

                /* encode current_command into code image */
                code_image[IC].command = binaryCommand;
                IC += command_offset + 1;


               /* printf("\t%lu current_command: %s length: %d\n", line_number, current_command.name, command_offset + 1); */
            } else
                continue;
        }
    }

    error_flag = error_flag | checkForUndefinedEntries(entry_list, entry_show_list, base_file_name);

    /* if all good  create entry and external files from lists */
    if (!error_flag) {
        info_file("No errors found, creating output files for", base_file_name);

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

    /* free all allocated memory */
    free(code_image);
    free(memory_image);
    free_list(label_list);
    free_list(entry_list);
    free_list(extern_list);
    free_list(entry_show_list);

    return error_flag;
}

/* Function: encodeArgumentToWord
 * Description: encodes an argument to a word according to its type
 * Parameters: token - the argument to encode
 *             current_arg_type - the type of the argument
 *             binArg - pointer to the word to encode to
 *             prev_register - the previous register if not exists it should hold -1
 *             label_list - list of labels
 *             extern_list - list of extern labels
 * Returns: error code if error occurred, 0 otherwise
 */
ERROR encodeArgumentToWord(char* token, arg_type current_arg_type, word** binArg, int prev_register, node_t* label_list, node_t* extern_list) {
    label_t* current_label;
    int data;

    switch (current_arg_type) {
        case Immediate:
            token++;
            (*binArg)->param.encoding_type = Absolute;
            data = atoi(token);
            (*binArg)->param.data = data;
            if (data > MAX_PARAM_VALUE || data < MIN_PARAM_VALUE) {
                return PARAM_VALUE_OUT_OF_RANGE;
            }
            break;
        case Direct:
            (*binArg)->param.encoding_type = Relocatable;
            if ((current_label = findLabel(token, label_list, extern_list, NULL)) == NULL) {
                return UNDEFINED_LABEL;
            }
            if (current_label->type == Extern) {
                (*binArg)->param.encoding_type = External;
                (*binArg)->param.data = 0;
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

        default:
            break;
    }
    return NO_ERROR;
}

/* Function: createEntryFile
 * Description: creates an entry file from the entry list and the base file name
 * Parameters: entry_show_list - the list of entry labels
 *             base_file_name - the base file name (without extension)
 * Output: none
 * Example: createEntryFile(entry_list, "test") will create a file named "test.ent" with the entry labels
 */
void createEntryFile(node_t* entry_show_list, char* base_file_name) {
    char* entry_file_name;
    FILE* output_entry_file;
    label_t* current_entry;

    if(!base_file_name) return;

    if (!entry_show_list) return;

    if(!(entry_file_name = getFileName(base_file_name, ENTRY_FILE_EXTENSION))) {
        return;
    }

    output_entry_file = fopen(entry_file_name, FILE_WRITE_MODE);

    while(entry_show_list) {
        current_entry = entry_show_list->data;
        fprintf(output_entry_file, "%s\t%lu\n",current_entry->name , current_entry->place);
        entry_show_list = (node_t *) entry_show_list->next;
    }

    fclose(output_entry_file);
    info_file("Created entry file", entry_file_name);
}

/* Function: createExternFile
 * Description: creates an extern file from the extern list and the base file name
 * Parameters: extern_show_list - the list of extern labels
 *             base_file_name - the base file name (without extension)
 * Output: none
 * Example: createExternFile(extern_list, "test") will create a file named "test.ext" with the extern labels
 */
void createExternFile(node_t* extern_show_list, char* base_file_name) {
    char* extern_file_name;
    FILE* output_extern_file;
    label_t* current_extern;

    if(!base_file_name) return;

    if (!extern_show_list) return;

    if(!(extern_file_name = getFileName(base_file_name, EXTERN_FILE_EXTENSION))) {
        return;
    }

    output_extern_file = fopen(extern_file_name, FILE_WRITE_MODE);

    while(extern_show_list) {
        current_extern = extern_show_list->data;
        fprintf(output_extern_file, "%s\t%lu\n",current_extern->name , current_extern->place);
        extern_show_list = (node_t *) extern_show_list->next;
    }

    fclose(output_extern_file);
    info_file("Created extern file", extern_file_name);
}

/* Function: createObjFile
 * Description: creates an object file from the code image, memory image and the base file name
 * Parameters: IC - the instruction counter (number of words in the code image)
 *             code_image - the code image (word array)
 *             DC - the data counter (number of words in the memory image)
 *             memory_image - the data image (word array)
 *             base_file_name - the base file name (without extension)
 * Output: none
 * Example: createObjFile(100, code_image, 200, data_image, "test") will create a file named "test.ob" with the object file
 */
void createObjFile(size_t IC, word* code_image, size_t DC, word* memory_image, char* base_file_name) {
    char* obj_file_name;
    FILE* output_obj_file;
    int i = 0;
    size_t current_word;

    if(!base_file_name) return;

    if(!(obj_file_name = getFileName(base_file_name, OBJ_FILE_EXTENSION))) {
        return;
    }

    output_obj_file = fopen(obj_file_name, FILE_WRITE_MODE);

    if(DEBUG) {
        fprintf(output_obj_file, "object file for: %s%s\n", base_file_name, ASM_FILE_EXTENSION);
    }
    fprintf(output_obj_file, "\t%lu\t%lu\n", IC, DC);

    if(DEBUG)
        fprintf(output_obj_file, "code image: \n");

    /* write code image */
    for (; i < IC; ++i) {
        current_word = code_image[i].data.data;
        fprintf(output_obj_file, "%04d\t", i + START_ADD);
        /* print data in binary 1 and 0 */
        writeObjToFile(current_word, WORD_SIZE, output_obj_file);
        fputc('\n', output_obj_file);
    }

    if(DEBUG)
        fprintf(output_obj_file, "memory image: \n");

    /* write memory image */
    for (i = 0; i < DC; ++i) {
        current_word = memory_image[i].data.data;
        fprintf(output_obj_file, "%04lu\t", i + START_ADD + IC);
        /* print data in binary 1 and 0 */
        writeObjToFile(current_word, WORD_SIZE, output_obj_file);
        fputc('\n', output_obj_file);
    }

    fclose(output_obj_file);
    info_file("Created object file", obj_file_name);
}

/* Function: checkForUndefinedEntries
 * Description: checks if there are undefined entries in the entry list and prints an error if there are
 * Parameters: entry_list - the list of entry labels
 *             entry_show_list - the list of entry labels that were defined
 *             base_file_name - the base file name (without extension)
 * Output: true if there are undefined entries, false otherwise
 * Example: checkForUndefinedEntries(entry_list, entry_show_list, "test") will return true if there are undefined entries
 */
bool checkForUndefinedEntries(node_t* entry_list, node_t* entry_show_list, char* base_file_name) {

    bool error_flag = false;

    while(entry_list != NULL) {
        if (!findLabelInList(((label_t*) entry_list->data)->name, entry_show_list)) {
            line_error(UNDEFINED_LABEL, base_file_name, ((label_t*) entry_list->data)->place, ((label_t*) entry_list->data)->name);
            error_flag = true;
        }
        entry_list = (node_t*) entry_list->next;
    }
    return error_flag;
}

