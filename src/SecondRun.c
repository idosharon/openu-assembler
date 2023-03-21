#include "SecondRun.h"


int second_run(int IC, int DC,
               node_t* label_list,
               node_t* extern_list,
               node_t* entry_list,
               bool error_flag,
               FILE* file,
               char* base_file_name) {

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

    int data, i = 0;

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
                /* encode all data */
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

                /* set params type to default None */
                binaryCommand.first_par_type = None;
                binaryCommand.second_par_type = None;

                /* set args type to default None */
                binaryCommand.dest_type = None;
                binaryCommand.src_type = None;

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
                        binaryCommand.src_type = encodeArgumentType(source_type);

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


                        binaryCommand.dest_type = encodeArgumentType(dest_type);

                        if(dest_type == Jump) {
                            /* get jump params and check the length of overall command */
                            arg_type first_param_type, second_param_type;

                            /* first param is second arg in command etc. */
                            if ((second_param_type = getJumpParamType(token, 1)) == None
                                    || (first_param_type = getJumpParamType(token, 2)) == None) {
                                error_flag = true;
                                continue;
                            }

                            binaryCommand.second_par_type = encodeArgumentType(second_param_type);
                            binaryCommand.first_par_type = encodeArgumentType(first_param_type);

                            command_length += getJumpParamsLength(token);
                        } else {
                            command_length++;
                        }
                    } else {
                        /* if expecting 2 args and not found raise error */
                        error_flag = true;
                        continue;
                    }
                }

                if(strtok(NULL, SPACE_SEP) != NULL) {
                    error_flag = true;
                    continue;
                }

                /* encode command into code image */
                code_image[IC].command = binaryCommand;

                /* code_image[++IC].param = 0; */
                IC += command_length;

                printf("\t%lu command: %s length: %d\n", line_number, command.name, command_length);
            } else {
                line_error(COMMAND_NOT_FOUND, base_file_name, line_number);
                continue;
            }
        }
    }

    /* print all command image and data image */
    printf("IC: %d DC: %d\n", IC, DC);
    for (i = 0; i < DC; ++i) {
        /* print command in binary 1 and 0 */
        data = memory_image[i].data.data;
        printf("DC: %d data: %d - ", i, data);
        /* print data in binary 1 and 0 */
        for (int j = 0; j < WORD_SIZE; ++j) {
            printf("%c", getBitRepresentation((data >> (WORD_SIZE-1 - j)) & 1));
        }
        printf("\n");
    }

    /* if all good  create entry and external files from lists */
    if(!error_flag) {
        printf("No errors found, creating output files: %s\n", base_file_name);
        if (entry_list != NULL) {
            /* create_entry_file(entry_list, base_file_name); */
        }
        if (extern_list != NULL) {
            /* create_extern_file(extern_list, base_file_name); */
        }
        createObjFile(IC, code_image, DC, memory_image, base_file_name);
    }
    return error_flag;
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

