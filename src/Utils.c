#include "Utils.h"


int appendBinaryToFile(FILE* fp, word binary) {
    if(!fp) return -1;

    while(binary.bits >>= 1) {
        fputc((binary.bits & 1 ? '1' : '0'), fp);
    }

    return 0;
}

word calculateOnsComplete(word* binary) {
    word neg_word;
    neg_word.bits = binary->bits;




    return neg_word;
}

char* reverse_string(char* str0) {
    char* str;
    int i;
    str = strdup(str0);
    int len = strlen(str);
    for (i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

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

int get_command_length(char* token, size_t command_index) {

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

int getJumpParamsLength(char* params_str) {
    int length = 0;
    params_str = strdup(params_str);

    if(!params_str) return -1;

    arg_type first_param_type = None;
    arg_type second_param_type = None;

    /* check label */
    params_str = strtok(params_str, JMP_PARAMS_SEP);
    if(!params_str || !isValidLabelFormat(params_str)) return -1;
    length++;

    /* check first param */
    params_str = strtok(NULL, JMP_PARAMS_SEP);
    if(!params_str || (first_param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None) return -1;
    length++;

    /* check second param */
    params_str = strtok(NULL, JMP_PARAMS_SEP);
    if(!params_str || (second_param_type = get_arg_type(params_str, Register | Immediate | Direct)) == None) return -1;
    length++;

    if(first_param_type == Register && second_param_type == Register) length--;

    return length;
}

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
