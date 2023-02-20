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
