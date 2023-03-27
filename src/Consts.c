/* File: Consts.c
 * Type: Source file
 * Description: Contains all the constants used in the assembler.
 * Authors: Ido Sharon (215774142)
 *          Amitai Ben Shalom (327743399)
 * Instructor: Ram Tahor
 * Course: C Programming Lab (20465)
 * Semester: 2023a
 */
#include "Consts.h"
#include "Types.h"

/* all the commands in an array of command_t, ordered by their binary codes */
const command_t commands[NUM_OF_COMMANDS] = {
        {"mov", Immediate | Direct | Register, Direct | Register},
        {"cmp", Immediate | Direct | Register, Immediate | Direct | Register},
        {"add", Immediate | Direct | Register, Direct | Register},
        {"sub", Immediate | Direct | Register, Direct | Register},

        {"not", 0, Direct | Register},
        {"clr", 0, Direct | Register},

        {"lea", Direct, Direct | Register},

        {"inc", 0, Direct | Register},
        {"dec", 0, Direct | Register},

        {"jmp", 0, Direct | Jump},
        {"bne", 0, Direct | Jump},

        {"red", 0, Direct | Register},
        {"prn", 0, Immediate | Direct | Register},

        {"jsr", 0, Direct | Jump},

        {"rts", 0, 0},
        {"stop", 0, 0}
};

/* all the registers in an array of strings, ordered by their binary codes */
const char* registers[NUM_OF_REGISTERS] = {
        "r0",
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7"
};
