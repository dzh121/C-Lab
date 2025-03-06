.entry MAIN
.extern STR
MAIN:       mov r3, LIST
            add #10, r2
            cmp r1, r2
            bne EXIT
            lea LABEL, r4
            jsr FUNCTION
            prn STR

LABEL:      .data 7, -3, 12
            .string "Hello"

FUNCTION:   inc r3
            dec r2
            rts

EXIT:       stop

LIST:       .data 20, -5, 8
