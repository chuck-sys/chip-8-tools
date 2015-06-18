## Chip-8 Assembler Documentation

___DISCLAIMER: All these mnemonics are created by the author, and (should) only work
with the Assembler that the author has created. It should not work for all assemblers.___

Here is a slightly large table of what works in the Chip-8 Assembler:

Mnemonic | Opcode | Usage | Description
---------|--------|-------|------------
CLS | 00E0 | `CLS` | Clears the entire screen; Sets the draw flag
RET | 00EE | `RET` | Returns from subroutine
JMP | 1nnn | `JMP 0x222` | Jumps to address nnn
CALL | 2nnn | `CALL 0x232` | Calls the subroutine at address nnn
SVE | 3xnn | `SVE 0x0, 0xa0` | Skips the next instruction if V[x] == nn
SVNE | 4xnn | `SVNE 0x0, 0xa0` | Skips the next instruction if V[x] == nn
SVVE | 5xy0 | `SVVE 0x0, 0xa` | Skips the next instruction if V[x] == V[y]
MOV | 6xnn | `MOV 0x0, 0x2a` | Sets the value of V[x] to nn
ADD | 7xnn | `ADD 0x0, 0x12` | Adds nn to the value of V[x] and stores it in V[x]
MOVR | 8xy0 | `MOVR 0x0, 0x1` | Sets the value of V[x] to the value of V[y]
ORR | 8xy1 | `ORR 0x0, 0x1` | Sets the value of V[x] to V[x] _OR_ V[y]

