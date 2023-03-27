; file: example10.as

.extern e1
.extern e2
.entry a1
.entry a2

MAIN: mov r3, LIST
inc e1
jmp LIST(e1,e2)
bne MAIN
dec e2
a1: sub #2,r0
    .data 1,2,3,4,5,6,7,8,9,10
a2: .string "abcdef"