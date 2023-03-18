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

int get_command_length(char* token) {
    /* check command type (group) */
    if(isStrEqual(token, "mov") || isStrEqual(token, "cmp") || isStrEqual(token, "add") ||
            isStrEqual(token, "sub") || isStrEqual(token, "lea")) {
        return 2;
    } else if(isStrEqual(token, "clr") || isStrEqual(token, "not") || isStrEqual(token, "inc") ||
            isStrEqual(token, "dec") || isStrEqual(token, "jmp") || isStrEqual(token, "bne") ||
            isStrEqual(token, "red") || isStrEqual(token, "prn") || isStrEqual(token, "jsr")) {
        return 1;
    } else if(isStrEqual(token, "rts") || isStrEqual(token, "stop")) {
        return 0;
    }
    token = strtok(NULL, SPACE_SEP);

    return 0;
}

int find_command(char* str) {
    int i;
    for(i = 0; i < NUM_OF_COMMANDS; i++) {
        if(isStrEqual(str, commands[i])) {
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
