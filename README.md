Chip-8 Toolset
--------------

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

The others may sometimes randomly segmentation fault, or may do something
unexpected, but I'm working on that.

The disassembler works on all chip-8 files.

The assembler works with the disassembler. Combining the 2 functions will result
in the same thing as the identity function, of course, with some space
limitations.

For more information, please see `docs/` or the [wiki][wiki].

To build these tools, please see `docs/Tutorials.txt` or the [wiki tutorials][wt].


[wiki]: https://github.com/cheukyin699/chip-8-tools/wiki
[wt]: https://github.com/cheukyin699/chip-8-tools/wiki/A-Quick-Tutorial
