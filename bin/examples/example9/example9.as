;file example9.as

LABEL1: .string "Hello World"
abc: cmp LABEL1,LABEL3
.extern LABEL3
    mcr mmm
    inc K
    jmp LABEL1(LABEL2,LABEL3)
    red r3
    prn #-5
    endmcr
    mmm
    mmm
    .entry LABEL2
LABEL2: stop
