#include "FirstRun.h"

void firstRun(FILE* file, char* base_file_name) {
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof (char));
    size_t line_number = 0;
    char* token;

    node_t* label_list = NULL;

    char* current_label = NULL;
    bool label_flag = false;

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

        /* check if there is a label */
        if(strchr(line,LABEL_SEP)) {
            if (isValidLabelName(token)) {
                label_flag = true;
                current_label = token;
                current_label[strlen(current_label)-1] = '\0';
                token = strtok(NULL,SPACE_SEP);
            } else {
                line_error(LABEL_SYNTAX_ERROR, base_file_name, line_number);
                continue;
            }
        }

        if (IS_DATA_SYMBOL(token)) {
            if (label_flag) {
                if (findLabel(current_label,label_list)) {
                    line_error(MULTIPLE_LABEL_DEFINITIONS, base_file_name, line_number);
                    continue;
                }
                else
                    label_list = addLabelNode(label_list, current_label, DC, Data);
            }
            if (isStrEqual(token, DATA_SYMBOL)) {
                /* calculate data length */
                while((token = strtok(NULL,COMMA_SEP)) != NULL) {
                    if (is_number(token)) {
                        DC++;
                    } else {
                        line_error(DATA_SYNTAX_ERROR, base_file_name,line_number);
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
            if(IS_EXTERN_SYMBOL(token)) {
                token = strtok(NULL,SPACE_SEP);
                if (token == NULL) {
                    line_error(EXTERN_MISSING_ARGUMENT,base_file_name,line_number);
                    continue;
                }
                if (!isValidLabelName(token)) {
                    line_error(LABEL_SYNTAX_ERROR,base_file_name,line_number);
                    continue;
                }
                extern_list = addLabelNode(extern_list, token, 0, Extern);
            }
            else {
                token = strtok(NULL,SPACE_SEP);
                if (token == NULL) {
                    line_error(ENTRY_MISSING_ARGUMENT,base_file_name,line_number);
                    continue;
                }
                if (!isValidLabelName(token)) {
                    line_error(LABEL_SYNTAX_ERROR,base_file_name,line_number);
                    continue;
                }
                entry_list = addLabelNode(entry_list, token, 0, Entry);
            }
        }
        else {
            if (label_flag) {
                if (findLabel(current_label, label_list)) {
                    line_error(MULTIPLE_LABEL_DEFINITIONS, base_file_name, line_number);
                    continue;
                } else {
                    label_list = addLabelNode(label_list, current_label, IC, Code);
                }
            }
            if (find_command(token) != -1) {
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

bool isValidLabelName(char* label_name) {
    /* check if label name is valid - only contains alphabetic and numbers */
    bool endLabel = false;
    if (!isalpha(*label_name))
        return false;
    for(; *label_name != '\0'; label_name++) {
        if (endLabel) return false;
        if (*label_name == LABEL_SEP) {
            endLabel = true;
        }
        else if(!isalpha(*label_name) && !isdigit(*label_name))
            return false;
    }
    if (find_register(label_name) != -1 || find_command(label_name) != -1) {
        return false;
    }
    return true;
}

node_t* addLabelNode(node_t* head, char* name, int place, label_type labelType) {
    /* set current label to the new label */
    label_t * new_label = (label_t*) (malloc(sizeof (label_t)));
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



/* commands binary strings */
char* getBinaryMov(char* token, char* base_file_name, int line_number) {

    /* build the binary string code from left to right and then reverse it */

    char *binary_str = (char *) (malloc(sizeof(char) * WORD_SIZE));
    char *operand1 = token;
    char *operand2;
    char *opcode = (char*) (malloc(sizeof(char) * OPCODE_SIZE));

    binary_str[0] = '\0';
    opcode[0] = '\0';
    operand1 = strtok(NULL, COMMA_SEP SPACE_SEP);
    operand2 = operand1;
    operand2 = strtok(NULL, COMMA_SEP SPACE_SEP);


    if (operand1 == NULL || operand2 == NULL) {
        line_error(COMMAND_SYNTAX_ERROR, base_file_name, line_number);
        return NULL;
    }
    if (find_register(operand1) != -1) {
        if (find_register(operand2) != -1) {
            /* mov r1,r2 */
            binary_str = strcat(binary_str, "001111");
            opcode = getOpcode(MOV);
            opcode = reverse_string(opcode);
            binary_str = strcat(binary_str, opcode);
            binary_str = strcat(binary_str, "0000");
            binary_str = reverse_string(binary_str);
            operand2 = strtok(NULL, COMMA_SEP);
            if (operand2 != NULL) {
                line_error(COMMAND_SYNTAX_ERROR, base_file_name, line_number);
                return NULL;
            }
            return binary_str;
        } else if (strchr(operand2, NUMBER_SYMBOL) != NULL) {
            line_error(INVALID_DEST_ARG, base_file_name, line_number);
            return NULL;
        } else if (isValidLabelName(operand2)) {
            /* mov r1, LABEL */
            binary_str = strcat(binary_str, "001011");
            opcode = getOpcode(MOV);
            opcode = reverse_string(opcode);
            binary_str = strcat(binary_str, opcode);
            binary_str = strcat(binary_str, "0000");
            binary_str = reverse_string(binary_str);
            operand2 = strtok(NULL, COMMA_SEP);
            if (operand2 != NULL) {
                line_error(COMMAND_SYNTAX_ERROR, base_file_name, line_number);
                return NULL;
            }
            return binary_str;
        } else {
            line_error(INVALID_DEST_ARG, base_file_name, line_number);
            return NULL;
        }
    } else if (strchr(operand1, NUMBER_SYMBOL) != NULL) {
        if (is_number(operand1 + 1)) {
            if (find_register(operand2) != -1) {
                /* mov #1,r1 */
                binary_str = strcat(binary_str, "001100");
                opcode = getOpcode(MOV);
                opcode = reverse_string(opcode);
                binary_str = strcat(binary_str, opcode);
                binary_str = strcat(binary_str, "0000");
                binary_str = reverse_string(binary_str);
                operand2 = strtok(NULL, COMMA_SEP);
                if (operand2 != NULL) {
                    line_error(COMMAND_SYNTAX_ERROR, base_file_name, line_number);
                    return NULL;
                }
                return binary_str;
            } else if (strchr(operand2, NUMBER_SYMBOL) != NULL) {
                line_error(INVALID_DEST_ARG, base_file_name, line_number);
                return NULL;
            } else if (isValidLabelName(operand2)) {
                /* mov #1, LABEL */
                binary_str = strcat(binary_str, "001000");
                opcode = getOpcode(MOV);
                opcode = reverse_string(opcode);
                binary_str = strcat(binary_str, opcode);
                binary_str = strcat(binary_str, "0000");
                binary_str = reverse_string(binary_str);
                operand2 = strtok(NULL, COMMA_SEP);
                if (operand2 != NULL) {
                    line_error(COMMAND_SYNTAX_ERROR, base_file_name, line_number);
                    return NULL;
                }
                return binary_str;

            } else {
                line_error(INVALID_DEST_ARG, base_file_name, line_number);
                return NULL;
            }
        } else {
            line_error(INVALID_SOURCE_ARG, base_file_name, line_number);
            return NULL;
        }
    } else if (isValidLabelName(operand1)) {
        if (find_register(operand2) != -1) {
            /* mov LABEL, r1 */
            binary_str = strcat(binary_str, "001110");
            opcode = getOpcode(MOV);
            opcode = reverse_string(opcode);
            binary_str = strcat(binary_str, opcode);
            binary_str = strcat(binary_str, "0000");
            binary_str = reverse_string(binary_str);
            operand2 = strtok(NULL, COMMA_SEP);
            if (operand2 != NULL) {
                line_error(COMMAND_SYNTAX_ERROR, base_file_name, line_number);
                return NULL;
            }
            return binary_str;
        } else if (strchr(operand2, NUMBER_SYMBOL) != NULL) {
            line_error(INVALID_DEST_ARG, base_file_name, line_number);
            return NULL;
        } else if (isValidLabelName(operand2)) {
            /* mov LABEL, LABEL */
            binary_str = strcat(binary_str, "001010");
            opcode = getOpcode(MOV);
            opcode = reverse_string(opcode);
            binary_str = strcat(binary_str, opcode);
            binary_str = strcat(binary_str, "0000");
            binary_str = reverse_string(binary_str);
            operand2 = strtok(NULL, COMMA_SEP);
            if (operand2 != NULL) {
                line_error(COMMAND_SYNTAX_ERROR, base_file_name, line_number);
                return NULL;
            }
            return binary_str;
        } else {
            line_error(INVALID_DEST_ARG, base_file_name, line_number);
            return NULL;
        }
    } else {
        line_error(INVALID_SOURCE_ARG, base_file_name, line_number);
        return NULL;
    }
}
