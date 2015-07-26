Chip-8 Toolset
--------------

![travis-ci][1]

This is a set of tools for emulating and looking at chip-8 files. It can
currently emulate the following without major problems:
- 15PUZZLE
- BLINKY
- BRIX
- INVADERS
- MAZE
- MISSILE (?)
- PONG
- PONG2
- TANK (?)
- TETRIS

The others may sometimes randomly seg. fault, or may do something unexpected,
but I'm working on that.

The assembler works on all chip-8 files. However, it may sometimes misrepresent
data as opcodes, which are also the same as the data once converted.

[1]: https://travis-ci.org/cheukyin699/chip-8-tools.svg?branch=master
