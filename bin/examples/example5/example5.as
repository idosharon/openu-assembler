; file: example5.as

    mov r7,r3
C: inc K
K: .string "HELLO WORLD"
    .extern C
    mov K,#2
    .entry LABEL
stop
