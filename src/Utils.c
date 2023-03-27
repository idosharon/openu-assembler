/*
 * File:   Utils.c
 * Type:   Source file
 * Description: Utility functions for the assembler.
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#include "Utils.h"

/* Function: find_command
 * Description: find command in const global commands array
 * Input: str - string (char*) to find
 * Output: index of command in commands array if found, ERROR_CODE otherwise
 */
int find_command(char* str) {
    int i;
    for(i = 0; i < NUM_OF_COMMANDS; i++) {
        if(isStrEqual(str, commands[i].name)) {
            return i;
        }
    }
    return ERROR_CODE;
}

/* Function: is_valid_label
 * Description: check if label name is valid (only contains alphabetic and numbers and starts with alphabetic)
 * Input: label_name - pointer to label name to check
 * Output: true if label name is valid, false otherwise
 */
bool isValidLabel(char* label_name) {
    /* copy label name */
    char* label_name_copy = strdup(label_name);
    /* check if label name is valid */
    if (!isValidLabelFormat(label_name_copy))
        return false;

    label_name_copy[strlen(label_name_copy) - 1] = '\0';
    if (find_register(label_name_copy) != ERROR_CODE
        || find_command(label_name_copy) != ERROR_CODE)
        return false;
    return true;
}

/* Function: isValidLabelFormat
 * Description: check if label name is valid (only contains alphabetic and numbers and starts with alphabetic)
 * Input: label_name - pointer to label name to check
 * Output: true if label name is valid, false otherwise
 * Example: isValidLabelFormat("label:") = true
 *          isValidLabelFormat("1label:") = false
 */
bool isValidLabelFormat(char* label_name) {
    /* end label flag - marks if end of label has been reached */
    bool endLabel = false;
    /* check if label starts with alphabetic */
    if (!isalpha(*label_name))
        return false;
    /* check if label contains only alphabetic and numbers */
    for(; *label_name != '\0'; label_name++) {
        /* check if found char after end of label (':') */
        if (endLabel) return false;
        /* check if label ends with ':' */
        if (*label_name == LABEL_SEP) {
            /* set end label flag */
            endLabel = true;
        }
            /* check if char is not alphabetic or number */
        else if(!isalpha(*label_name) && !isdigit(*label_name))
            return false;
    }
    /* label is valid return true */
    return true;
}

/* Function: getJumpParamType
 * Description: get the type of specific param in a jump command
 * Input: params_str - pointer to entire jump command string ("L1(r0, #5)")
 *        param_number - param number to get type of (1 or 2)
 * Output: param type if valid, None otherwise
 * Example: getJumpParamType("L1(r0, #5)", 1) = Register
 */
arg_type getJumpParamType(char* params_str, int param_number) {
    arg_type param_type;

    params_str = strdup(params_str);

    if(!params_str) return None;

    /* check label */
    params_str = strtok(params_str, JMP_OPEN_BRACKET);
    if(!params_str || !isValidLabelFormat(params_str)) return None;

    /* check first param */
    switch (param_number) {
        case 1:
            params_str = strtok(NULL, COMMA_SEP);
            if (!params_str
                || (param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None)
                return None;
            break;
        case 2:
            params_str = strtok(NULL, COMMA_SEP);
            params_str = strtok(NULL, JMP_CLOSE_BRACKET);
            if (!params_str
                || (param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None)
                return None;
            break;
        default:
            return None;
    }

    return param_type;
}

/* Function: getJumpParamsLength
 * Description: get the length of a jump command
 * Input: params_str - pointer to entire jump command string ("L1(r0, #5)")
 * Output: length of jump command if valid, -1 otherwise
 * Example: getJumpParamsLength("L1(r0, #5)") = 4
 *          getJumpParamsLength("L1(r0, r1)") = 3
 */
int getJumpParamsLength(char* params_str) {
    int length = 0;
    arg_type first_param_type;
    arg_type second_param_type;

    /* get first & second param type */
    if ((first_param_type = getJumpParamType(params_str, 1)) == None
            || (second_param_type = getJumpParamType(params_str, 2)) == None)
        return -1;
    /* if both valid params, add 3 to length */
    length += 3;

    /* if both params are registers, subtract 1 from length */
    if (first_param_type == Register && second_param_type == Register)
        length--;

    return length;
}

/* Function: get_arg_type
 * Description: get the type of given argument (Register, Immediate, Direct, Jump)
 * Input: token - pointer to argument string
 *        types - types to check for
 * Output: argument type if valid, None otherwise
 * Example: get_arg_type("#5", Jump) = None
 *          get_arg_type("r0", Register | Direct) = Register
 *          get_arg_type("L1", Direct | Jump) = Direct
 *          get_arg_type("L1(r0, #5)", Jump) = Jump
 */
arg_type get_arg_type(char* token, arg_type types) {
    if(find_command(token) != ERROR_CODE) {
        /* command is preserved name */
        return None;
    }

    /* r0 = Register, #(num) = Immediate, Label = Direct, Label(..., ...) = Jump */
    if((types & Immediate)
            && (token[0] == IMMEDIATE_PREFIX && is_number(token + 1))) {
        return Immediate;
    } else if((types & Jump) && (getJumpParamsLength(token) != ERROR_CODE)) {
        return Jump;
    } else if((types & Register) && (find_register(token) != ERROR_CODE)) {
        return Register;
    }  else if((types & Direct) && (isValidLabelFormat(token))) {
        return Direct;
    }

    return None;
}

/* Function: find_register
 * Description: find a register in the registers array
 * Input: str - register name to find
 * Output: register index if found, ERROR_CODE otherwise
 */
int find_register(char* str) {
    int i;
    for(i = 0; i < NUM_OF_REGISTERS; i++) {
        if(isStrEqual(str, registers[i])) {
            return i;
        }
    }
    return ERROR_CODE;
}

/* Function: findLabelInList
 * Description: find a label in a list of labels
 * Input: name - label name to find
 *        head - head of list to search in
 * Output: the found label if found, NULL otherwise
 */
label_t* findLabelInList(char* name, node_t* head) {
    label_t* label;
    while(head != NULL) {
        if(isStrEqual((label = (label_t*) head->data)->name, name)) {
            return label;
        }
        head = (node_t*) head->next;
    }
    return NULL;
}

/* Function: findLabel
 * Description: find a label in a list of labels
 * Input: name - label name to find
 *        label_list - list of labels to search in
 *        ... - NULL terminated list of label lists to search in
 * Output: label if found, NULL otherwise
 * Example: findLabel("L1", label_list, NULL) = label_t*
 *          findLabel("L1", label_list, label_list2, NULL) = label_t*
 *          findLabel("L1", label_list, label_list2, label_list3, NULL) = label_t*
 *          findLabel("L1", label_list, label_list2, label_list3, label_list4, NULL) = label_t*
 */
label_t* findLabel(char* name, node_t* label_list, ...) {
    label_t* label;
    va_list lists;
    va_start(lists, label_list);
    while(label_list != NULL) {
        if((label = findLabelInList(name, label_list)) != NULL) {
            return label;
        }
        label_list = va_arg(lists, node_t*);
    }
    va_end(lists);
    return NULL;
}
/* Function: addLabelNode
 * Description: add a new label to the start of a given list
 * Input: head - pointer to the head of the label list
 *        name - label name
 *        place - label address
 *        labelType - label type
 * Output: pointer to the new head of the label list
 */
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

/* Function: updateDCInList
 * Description: update the DC address of all data labels in a given list
 * Input: start_add - the new DC address
 *        head - pointer to the head of the label list
 * Output: None
 */
void updateDCInList(size_t IC, node_t* head) {
    label_t* label;
    while(head != NULL) {
        label = (label_t*) head->data;
        if(label->type == Data) {
            label->place += IC;
        }
        head = (node_t*) head->next;
    }
}
/* Function: updateDC
 * Description: update the DC address of all data labels in a given list
 * Input: IC - the new DC address
 *        label_list - pointer to the head of the label list
 *        ... - NULL terminated list of label lists
 * Output: None
 */
void updateDC(size_t IC, node_t* label_list, ...) {
    va_list lists;
    va_start(lists, label_list);
    while(label_list != NULL) {
        updateDCInList(IC, label_list);
        label_list = va_arg(lists, node_t*);
    }
}

/* Function: updateICInList
 * Description: update the IC address of all code labels in a given list
 * Input: start_add - the new IC address
 *        head - pointer to the head of the label list
 * Output: None
 */
void updateICInList(int start_add, node_t* head) {
    label_t* label;
    while(head != NULL) {
        label = (label_t*) head->data;
        if(label->type == Code) {
            label->place += start_add;
        }
        head = (node_t*) head->next;
    }
}

/* Function: updateIC
 * Description: update the IC address of all code labels
 * Input: IC - the new IC address
 *        label_list - pointer to the head of the label list
 *        ... - additional label lists
 * Output: None
 */
void updateIC(int IC, node_t* label_list, ...) {
    va_list lists;
    va_start(lists, label_list);
    while(label_list != NULL) {
        updateICInList(IC, label_list);
        label_list = va_arg(lists, node_t*);
    }
}

/* Function: free_list
 * Description: free all nodes in a list
 * Input: head - pointer to the head of the list
 * Output: None
 */
void free_list(node_t* head) {
    /* free list */
    node_t* current = head;
    node_t* next;

    /* free each node */
    while(current != NULL) {
        next = (node_t *) current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

/* Function: getFileName
 * Description: get a file name from a base name and an extension
 * Input: base - the base name of the file
 *        ext - the extension of the file
 * Output: a pointer to the file name
 *         NULL if an error occurred
 */
char *getFileName(char* base, char* ext) {
    char* file_name;
    if(!base || !ext) return NULL;

    file_name = (char*) malloc(strlen(base) + strlen(ext) + 1);

    if(file_name) {
        strcpy(file_name, base);
        strcat(file_name, ext);
        return file_name;
    }
    return NULL;
}

/* Function: openFile
 * Description: open a file
 * Input: file_name - the name of the file to open
 *        mode - the mode to open the file in
 * Output: a pointer to the file
 *         NULL if an error occurred
 */
FILE* openFile(char* file_name, char* mode) {
    FILE* fp;
    if(!file_name || !mode) return NULL;

    fp = fopen(file_name, mode);

    if(!fp) {
        file_error(FILE_OPEN_ERROR, file_name);
        return NULL;
    }

    return fp;
}

/* Function: writeObjToFile
 * Description: write a word to the object file
 * Input: current_word - the word to write
 *        num_of_bits - number of bits to write
 *        fp - file pointer to write to
 * Output: none
 */
void writeObjToFile(size_t current_word, size_t num_of_bits, FILE* fp) {
    int j;
    /* write each bit in the word to the file */
    for (j = 0; j < num_of_bits; ++j) {
        /* write the bit to the file */
        fputc(getBitRepresentation((current_word >> (num_of_bits - 1 - j)) & 1),fp);
    }
}


/* Function: is_number
 * Description: check if string is a number
 * Input: str - string (char*) to check
 * Output: true if string is a number, false otherwise
 */
bool is_number(char* str) {
    int i = 0;
    if(!str) return false;
    if (*str == '-' || *str == '+')
        str++;
    for(; i < strlen(str); i++) {
        if(!isdigit(str[i])) return false;
    }
    return true;
}

/* Function: to_number
 * Description: convert a string to a number with 2's complement support and error checking
 * Input: str - string (char*) to convert
 *        num - pointer to the number to write to
 * Output: NO_ERROR if no error occurred, ERROR_CODE otherwise
 */
ERROR to_number(char* str, int* num) {
    int i = 0;
    bool number_started = false;
    int sign = 1;

    if(!str) return 0;
    *num = 0;

    for(; i < strlen(str); i++) {
        if(isdigit(str[i])) {
            number_started = true;
            *num = (*num) * 10 + (str[i] - '0');
        }
        else if(!number_started && str[i] == '-') {
            sign *= -1;
        }
        else {
            return ERROR_CODE;
        }
    }

    if(sign == -1) {
        *num = ~(*num) + 1;
    }

    return NO_ERROR;
}
