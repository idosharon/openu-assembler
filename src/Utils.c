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
    char* label_name_copy = strdup(label_name);
    if (!isValidLabelFormat(label_name_copy))
        return false;
    label_name_copy[strlen(label_name_copy) - 1] = '\0';
    if (find_register(label_name_copy) != -1
        || find_command(label_name_copy) != -1)
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
    arg_type param_type = None;

    params_str = strdup(params_str);

    if(!params_str) return None;

    /* check label */
    params_str = strtok(params_str, JMP_OPEN_BRACKET);
    if(!params_str || !isValidLabelFormat(params_str)) return None;

    /* check first param */
    if (param_number == 1) {
        params_str = strtok(NULL, COMMA_SEP);
        if (!params_str
                || (param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None)
            return None;
    }
        /* check second param */
    else if (param_number == 2) {
        params_str = strtok(NULL, COMMA_SEP);
        params_str = strtok(NULL, JMP_CLOSE_BRACKET);
        if (!params_str
                || (param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None)
            return None;
    }

    return param_type;
}


int getJumpParamsLength(char* params_str) {

    int length = 0;
    arg_type first_param_type;
    arg_type second_param_type;

    if ((first_param_type = getJumpParamType(params_str, 1)) == None
            || (second_param_type = getJumpParamType(params_str, 2)) == None)
        return -1;
    length += 3;
    if (first_param_type == Register && second_param_type == Register)
        length--;

    return length;
}

arg_type get_arg_type(char* token, arg_type types) {
    if(find_command(token) != -1) {
        /* command is preserved name */
        return None;
    }

    /* r0 = Register, #(num) = Immediate, Label = Direct, Label(..., ...) = Jump */
    if((types & Immediate)
            && (token[0] == IMMEDIATE_PREFIX && is_number(token + 1))) {
        /* TODO: check if number if in range */
        return Immediate;
    } else if((types & Jump) && (getJumpParamsLength(token) != -1)) {
        return Jump;
    } else if((types & Register) && (find_register(token) != -1)) {
        return Register;
    }  else if((types & Direct) && (isValidLabelFormat(token))) {
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
    va_end(lists);
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

/* update the DC address of all data labels */
void updateDC(size_t IC, node_t* label_list, ...) {
    label_t* label;
    va_list lists;
    va_start(lists, label_list);
    while(label_list != NULL) {
        updateDCInList(IC, label_list);
        label_list = va_arg(lists, node_t*);
    }
}

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

/* update the IC address of all data labels */
void updateIC(int IC, node_t* label_list, ...) {
    label_t* label;
    va_list lists;
    va_start(lists, label_list);
    while(label_list != NULL) {
        updateICInList(IC, label_list);
        label_list = va_arg(lists, node_t*);
    }
}

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

void writeBinToFile(size_t current_word, size_t num_of_bits, FILE* fp) {
    int j = 0;
    for (j = 0; j < num_of_bits; ++j) {
        fputc(getBitRepresentation((current_word >> (num_of_bits - 1 - j)) & 1),fp);
    }
}


