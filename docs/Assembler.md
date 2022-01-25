### Chip-8 Assembler Syntax

Here is a slightly large table of what works in the Chip-8 Assembler:

Mnemonic | Opcode | Usage | Description
---------|--------|-------|------------
CLS | 00E0 | `CLS` | Clears the entire screen; Sets the draw flag
RET | 00EE | `RET` | Returns from subroutine
JP | 1nnn | `JP start` | Jumps to address nnn
CALL | 2nnn | `CALL foo` | Calls the subroutine at address nnn
SE | 3xnn | `SE V0, 0xa0` | Skips the next instruction if V[x] == nn
SNE | 4xnn | `SNE V0, 0xa0` | Skips the next instruction if V[x] == nn
SE | 5xy0 | `SE V0, Va` | Skips the next instruction if V[x] == V[y]
LD | 6xnn | `LD V0, 0x2a` | Sets the value of V[x] to nn
ADD | 7xnn | `ADD V0, 0x12` | Adds nn to the value of V[x] and stores it in V[x]
LD | 8xy0 | `LD V0, V1` | Sets the value of V[x] to the value of V[y]
OR | 8xy1 | `OR V0, V1` | Sets the value of V[x] to V[x] _OR_ V[y]
AND | 8xy2 | `AND V0, V1` | Sets the value of V[x] to V[x] _AND_ V[y]
XOR | 8xy3 | `XOR V0, V1` | Sets the value of V[x] to V[x] _XOR_ V[y]
ADD | 8xy4 | `ADD V0, V1` | Sets V[x] to V[x] + V[y]; Set V[0xf] = 1 if there is carry and vice versa
SUB | 8xy5 | `SUB V0, V1` | Sets V[x] to V[x] - V[y]; Set V[f] = 0 if there is borrow and vice versa
SHR | 8x06 | `SHR V0` | Saves LSB of V[x] in V[0xf]; Shifts V[x] to the right by 1 bit
SUBN | 8xy7 | `SUBN V0, V1` | Sets V[x] to V[y] - V[x]; Set V[f] = 0 if there is borrow and vice versa
SHL | 8x0e | `SHL V0` | Saves MSB of V[x] in V[0xf]; Shifts V[x] to the left by 1 bit
SNE | 9xy0 | `SNE V0, V1` | Skips the next instruction if V[x] != V[y]
LD | Annn | `LD I, 0x29a` | Sets index register I to nnn
JP | Bnnn | `JP V0, flist` | Jumps to address nnn+V[0]
RND | Cxnn | `RND V0, 0x1` | Sets V[x] to a random number masked by nn
DRW | Dxyn | `DRW V0, V1, 0x2` | Reads n bytes from memory, and displays them at (V[x], V[y]); Sets V[0xf] to whether any pixel collided (collided? 1:0)
SKP | Ex9E | `SKP V0` | Skips the next instruction if key[V[x]] is pressed
SKNP | ExA1 | `SKNP V0` | Skips the next instruction if key[V[x]] is _NOT_ pressed
LD | Fx07 | `LD V0, DT` | Sets V[x] to the value in delay timer DT
LD | Fx0A | `LD V0, K` | Waits for a key press, then stores the value of pressed key in V[x]
LD | Fx15 | `LD DT, V0` | Sets DT to the value in V[x]
LD | Fx18 | `LD ST, V0` | Sets ST sound timer to the value in V[x]
ADD | Fx1E | `ADD I, V0` | Sets I to I + V[x]
LDI | Fx29 | `LDI V0` | Sets index register I to the location of the sprite for digit V[x]
BCD | Fx33 | `BCD V0` | Stores BCD representation of V[x] in memory location I, I+1, I+2 (hundreds, tens, ones)
PUSH | Fx55 | `PUSH Vf` | Pushes all registers from V[0] to V[x] inclusive to location I, I+1, ...
POP | Fx65 | `POP Vf` | Pops all registers from V[0] to V[x] inclusive from location I, I+1, ...

### Syntax

```asm
; Infinite loop
start:
begin: JP start
```

Comments are one-liners starting with `;` character.

```asm
LD I, sprite1       ; the same thing
LD I, 0x206         ; the same thing
DRW V0, V1, 0x2

sprite1:
; To insert data, just have the number there;
; automatic insertion will take place
0x000f 0x0000 0x9
; the above creates 6 bytes of data
```

Only mnemonics that are NNN can have the address replaced with a label. This includes the following:

```asm
; Obviously
start: JP start
call start
LD I, start
JP V0, start
```

And you can also replace the label with some address.

### Usage

```
./Chip8_Assembler [options] <filename>

Options:
  -o <file>
  --out <file>             Place the output into <file>

  -h, --help               Shows this help text

  -1p, --one-pass          Makes 1 pass instead of 2, halving compilation time, but
                           only use this option if you know you don't have any labels
                           that are used before they are declared.
```

### Output

With no options, the assembler outputs the compiled code into file `a.out`,
passing over it twice.

If you don't have labels in code that are used before defined (post-defined?),
or don't have labels in general, you may also add option `-1p` to make compiling
faster by making 1 pass instead of 2.

To specify the filename for the compiled code (and to avoid the step of renaming
file `a.out` to something else), use option `-o <filename>`.
