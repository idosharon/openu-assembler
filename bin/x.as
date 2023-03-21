; test file 1

MAIN:   mov r3, L1
LOOP:   jmp L1(#-1,r6)
        mcr m1
        sub r1, r4
        bne W(r4, r5)
        endmcr
.entry L1
        prn #-5
        .extern W
        mcr amitai
            hello world
        endmcr
        m1
        bne LOOP(r4, r3)
        m1
L1:     inc K
        bne LOOP(K, STR)
END:    stop
STR:    .string "abcdef"
LENGTH: .data 6, -9,15
K:      .data 22