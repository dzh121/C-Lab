.extern W
.entry LIST
MAIN:   add r3, LIST
LOOP:   prn #48
        mcro a_mc
        bne W
        sub r1, r4
        bne L1
        mcroend
        lea STR, r6
        inc r6
        mov r3, K
        sub r1, r4
        bne END
        cmp K, #-6
        bne &END
        dec K
        a_mc
        jmp &LOOP
END:    stop
STR:    .string "abcd"
LIST:   .data 6, -9
        .data -100
K:      .data 31