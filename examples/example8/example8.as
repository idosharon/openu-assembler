;file: example8.as

    mcr ex8
    jmp K(L1,L2)
    clr L1
    inc K
    endmcr
DATA1: .data 1,2,-2
    red r1
    red r2
    ex8
    ex8
    stop

