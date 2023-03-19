#include "Utils.h"




bool is_number(char* str) {
    /* TODO: check if number is in range */
    int i = 0;
    if(!str) return false;
    if (*str == '-' || *str == '+')
        str++;
    for(; i < strlen(str); i++) {
        if(!isdigit(str[i])) return false;
    }
    return true;
}

bool isValidLabel(char* label_name) {
    /* check if label name is valid - only contains alphabetic and numbers */
    if (!isValidLabelFormat(label_name)
        || find_register(label_name) != -1
        || find_command(label_name) != -1)
        return false;
    return true;
}

bool isValidLabelFormat(char* label_name) {
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
    return true;
}


arg_type getJumpParamType(char* params_str, int param_number) {
    params_str = strdup(params_str);

    if(!params_str) return -1;
    arg_type param_type = None;

    /* check label */
    params_str = strtok(params_str, JMP_OPEN_BRACKET);
    if(!params_str || !isValidLabelFormat(params_str)) return -1;

    /* check first param */
    if (param_number == 1) {
        params_str = strtok(NULL, COMMA_SEP);
        if (!params_str || (param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None)
            return -1;
    }
        /* check second param */
    else if (param_number == 2) {
        params_str = strtok(NULL, COMMA_SEP);
        params_str = strtok(NULL, JMP_CLOSE_BRACKET);
        if (!params_str || (param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None)
            return -1;
    }

    return param_type;
}


int getJumpParamsLength(char* params_str) {

    int length = 0;
    arg_type first_param_type = None;
    arg_type second_param_type = None;

    first_param_type = getJumpParamType(params_str, 1);
    second_param_type = getJumpParamType(params_str, 2);
    if (first_param_type == -1 || second_param_type == -1)
        return -1;
    length += 3;
    if (first_param_type == Register && second_param_type == Register)
        length--;

    return length;
}
/*
    params_str = strdup(params_str);

    if(!params_str) return -1;




    params_str = strtok(params_str, JMP_OPEN_BRACKET);
    if(!params_str || !isValidLabelFormat(params_str)) return -1;
    length++;

    params_str = strtok(NULL, COMMA_SEP);
    if(!params_str || (first_param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None) return -1;
    length++;

    params_str = strtok(NULL, JMP_CLOSE_BRACKET);
    if(!params_str || (second_param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None) return -1;
    length++;

    if(first_param_type == Register && second_param_type == Register) length--;

    return length;
}
*/

arg_type get_arg_type(char* token, arg_type types) {
    if(find_command(token) != -1) {
        /* command is preserved name */
        return None;
    }

    /* r0 = Register, #(num) = Immediate, Label = Direct, Label(..., ...) = Jump */
    if((types & Immediate)
            && (token[0] == '#' && is_number(token + 1))) {
        return Immediate;
    } else if((types & Jump) && (getJumpParamsLength(token) != -1)) {
        return Jump;
    } else if((types & Register) && (find_register(token) != -1)) {
        return Register;
    } else if((types & Direct) && (isValidLabelFormat(token))) {
        return Direct;
    }

    return None;
}

int find_command(char* str) {
    int i;
    for(i = 0; i < NUM_OF_COMMANDS; i++) {
        if(isStrEqual(str, commands[i].name)) {
            return i;
        }
    }
    return -1;
}

int find_register(char* str) {
    int i;
    for(i = 0; i < NUM_OF_REGISTERS; i++) {
        if(isStrEqual(str, registers[i])) {
            return i;
        }
    }
    return -1;
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
    return NULL;
}

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

char* getRegisterNumber(int index) {
    switch (index) {
        case 0: return "000";
        case 1: return "001";
        case 2: return "010";
        case 3: return "011";
        case 4: return "100";
        case 5: return "101";
        case 6: return "110";
        case 7: return "111";
        default: return NULL;
    }
}

char* getOpcode(int index) {
    switch (index) {
        case 0: return "0000";
        case 1: return "0001";
        case 2: return "0010";
        case 3: return "0011";
        case 4: return "0100";
        case 5: return "0101";
        case 6: return "0110";
        case 7: return "0111";
        case 8: return "1000";
        case 9: return "1001";
        case 10: return "1010";
        case 11: return "1011";
        case 12: return "1100";
        case 13: return "1101";
        case 14: return "1110";
        case 15: return "1111";

        default: return NULL;
    }
}

void free_list(node_t* head) {
    /* free list */
    node_t* current = head;
    node_t* next;

    while(current != NULL) {
        next = (node_t *) current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

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


