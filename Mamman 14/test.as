MAIN:   add r3, LIST
LOOP:   prn #48 ;hello
        fjsd
        mcro   a_mc
        cmp K, #-6
        bne &END
        mcroend
        lea STR, r6
        inc r6
        mcro b_mc
        add r3, r1
        sub r1, r4
        mcroend
        mov r3, K
        sub r1, r4
        bne END
        a_mc
        dec K
        jmp &LOOP
        b_mc
        b_mc
END:    stop
STR:    .string "abcd"
LIST:   .data 6, -9
        .data -100
K:      .data 31