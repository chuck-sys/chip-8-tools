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
ANR | 8xy2 | `ANR 0x0, 0x1` | Sets the value of V[x] to V[x] _AND_ V[y]
XORR | 8xy3 | `XORR 0x0, 0x1` | Sets the value of V[x] to V[x] _XOR_ V[y]
ADC | 8xy4 | `ADC 0x0, 0x1` | Sets V[x] to V[x] + V[y]; Set V[0xf] = 1 if there is carry and vice versa
SBB | 8xy5 | `SBB 0x0, 0x1` | Sets V[x] to V[x] - V[y]; Set V[f] = 0 if there is borrow and vice versa
SRS | 8x06 | `SRS 0x0` | Saves LSB of V[x] in V[0xf]; Shifts V[x] to the right by 1 bit
SRB | 8xy7 | `SRB 0x0, 0x1` | Sets V[x] to V[y] - V[x]; Set V[f] = 0 if there is borrow and vice versa
SLS | 8x0e | `SLS 0x0` | Saves MSB of V[x] in V[0xf]; Shifts V[x] to the left by 1 bit
SVVNE | 9xy0 | `SVVNE 0x0, 0x1` | Skips the next instruction if V[x] != V[y]
SETI | Annn | `SETI 0x29a` | Sets index register I to nnn
JAR | Bnnn | `JAR 0x29b` | Jumps to address nnn+V[0]
RNRM | Cxnn | `RNRM 0x0, 0x01` | Sets V[x] to a random number masked by nn
DSP | Dxyn | `DSP 0x0, 0x1, 0x2` | Reads n bytes from memory, and displays them at (V[x], V[y]); Sets V[0xf] to whether any pixel collided (collided? 1:0)
SKP | Ex9E | `SKP 0x0` | Skips the next instruction if key[V[x]] is pressed
SKNP | ExA1 | `SKNP 0x0` | Skips the next instruction if key[V[x]] is _NOT_ pressed
SRD | Fx07 | `SRD 0x0` | Sets V[x] to the value in delay timer DT
WKP | Fx0A | `WKP 0x0` | Waits for a keypress, then stores the value of pressed key in V[x]
SDR | Fx15 | `SDR 0x0` | Sets DT to the value in V[x]
SSR | Fx18 | `SSR 0x0` | Sets ST sound timer to the value in V[x]
SETH | Fx29 | `SETH 0x0` | Sets index register I to the location of the sprite for digit V[x]
BCD | Fx33 | `BCD 0x0` | Stores BCD representation of V[x] in memory location I, I+1, I+2 (hundreds, tens, ones)
PUSHA | Fx55 | `PUSHA 0xf` | Pushes all registers from V[0] to V[x] inclusive to location I, I+1, ...
POPA | Fx65 | `POPA 0xf` | Pops all registers from V[0] to V[x] inclusive from location I, I+1, ...


### Extra Commands

Mnemonic | Opcode | Usage | Description
---------|--------|-------|------------
DATA | _NONE_ | `DATA 0x0001, 0x0009, 0x9a83, ...` | Inserts raw data (the arguments) into the resulting binary
