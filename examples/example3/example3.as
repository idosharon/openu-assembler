; file: example3.as

    .extern STR1
LABEL1: mov #-100, STR1
    jmp LABEL2
    not r0
    .entry STR2
    clr STR2
    mov #-100, STR2
STR2: .string "abc def"
    .data -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    stop