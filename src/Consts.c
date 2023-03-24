#include "Consts.h"
#include "Types.h"

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
