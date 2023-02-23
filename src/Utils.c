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

int get_command_length(int index) {
    switch (index) {
        case 0: case 1: case 2: case 3:
            return 3;
        case 4: case 5: case 7: case 8: case 12:
            return 2;
        case 15:
            return 1;
        default: return 4;
    }
}

int find_command(char* str) {
    int i;
    for(i = 0; i < NUM_OF_COMMANDS; i++) {
        if(is_equal(str, commands[i])) {
            return i;
        }
    }
    return -1;
}
