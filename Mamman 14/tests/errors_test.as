; Assembly file with multiple errors for testing assembler error handling

; Duplicate label with different types
LABEL1: .data 5
LABEL1: .string "Duplicate"

; Label with invalid characters
LABEL@INVALID: add r1, r2

; Missing macro definition before usage
CALLMACRO

; Invalid instruction format
mov, r1, r2

; Missing operand
add r1,

; Extraneous text after instruction
cmp r2, r3 extratext

; Unrecognized directive
.fake_directive 123

; Invalid data declaration
.data 1, , 3

; String missing closing quotes
.string "unterminated

; Undefined label usage
jmp UNKNOWNLABEL

; Using .entry on an undefined label
.entry UNDEFINEDENTRY

; Using .extern on an already defined label
.extern LABEL2
LABEL2: .data 10

; Invalid addressing mode
mov #5, #10

; Attempting to use an invalid register
mov r8, r1

; Using a register with an invalid addressing mode
lea r2, r3

; Missing comma between operands
sub r1 r2

; Multiple consecutive commas
add r1,,r2

; Unmatched parentheses
prn (r1

; Invalid ARE bit combination
mov &r1, r2

; Trying to jump to an immediate value
jmp #100

; Trying to use an entry label as an extern
.entry LABEL3
.extern LABEL3

