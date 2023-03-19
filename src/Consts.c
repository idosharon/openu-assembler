#include "Consts.h"
#include "Types.h"

const command_t commands[NUM_OF_COMMANDS] = {
        {"mov", Immediate | Direct | Register, Direct | Register, TWO_ARGS, false},
        {"cmp", Immediate | Direct | Register, Immediate | Direct | Register, TWO_ARGS, false},
        {"add", Immediate | Direct | Register, Direct | Register, TWO_ARGS, false},
        {"sub", Immediate | Direct | Register, Direct | Register, TWO_ARGS, false},

        {"not", 0, Direct | Register, ONE_ARGS, false},
        {"clr", 0, Direct | Register, ONE_ARGS, false},

        {"lea", Direct, Direct | Register, TWO_ARGS, false},

        {"inc", 0, Direct | Register, ONE_ARGS, false},
        {"dec", 0, Direct | Register, ONE_ARGS, false},

        {"jmp", 0, Direct | Jump, ONE_ARGS, true},
        {"bne", 0, Direct | Jump, ONE_ARGS, true},

        {"red", 0, Direct | Register, ONE_ARGS, false},
        {"prn", 0, Immediate | Direct | Register, ONE_ARGS, false},

        {"jsr", 0, Direct | Jump, ONE_ARGS, true},

        {"rts", 0, 0, ZERO_ARGS, false},
        {"stop", 0, 0, ZERO_ARGS, false}
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
