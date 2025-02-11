; Correct assembly code with valid syntax

.entry START
.extern EXTLABEL

START:   mov r1, r2
         add #5, r3
         lea DATA, r4
         sub r2, r4
         cmp r3, r4
         jmp LOOP

LOOP:    prn #20
         inc r2
         bne END
         jsr FUNC
         rts

FUNC:    red r5
         not r5
         clr r6
         dec r6
         dec EXTLABEL
         stop

DATA:    .data 10, -3, 7
STRING:  .string "hello"

END:     stop