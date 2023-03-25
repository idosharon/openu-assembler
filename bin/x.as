; file x.as
    .entry LENGTH
    .extern W
    mov r3,STR
MAIN: mov r3 ,LENGTH
LOOP: jmp L1(W,r6)
    prn #-5
    bne W(r4,L4)
    sub r1, r4
    bne L3
    mcr fff
    L4: .data 6
    endmcr
L1: inc K
    .entry LOOP
    fff
    jmp END(END,END)
    bne LOOP(K,W)
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-1, 2000
K: .data 22
    .extern L3