; file: example6.as

.entry a
.entry b
.entry c
.extern d
.extern e
.extern f

mov r0,r2
cmp d,e
bne L3,L4
jmp L3,L4

a: clr f
b: jmp a
c: jmp b

L3: .data 1,2,3
L4: .string "example6.as"