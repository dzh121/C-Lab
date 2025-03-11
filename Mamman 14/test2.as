START:      mov r1, r2
            add r3, r4
            sub r5, r6
            not r7
LABEL1:     lea LABEL2, rhe
            jmp LABEL1
LABEL2:     bne LABEL1

DATASEC:   .data 5, -3, 10
            .string "Hello"

            mov #5, LABEL3
            add r8, r2
            .data "string"
            mov r1
            add r2,
            sub ,r3
            mov LABEL1, #4
            prn "text"
            mov #3.14, r2
            .data 5.6, -2.3
            mov r1, r2 extra_text_here
            jmp very_very_long_label_name_that_exceeds_31_characters

            jmp r3
            bne r4

stop