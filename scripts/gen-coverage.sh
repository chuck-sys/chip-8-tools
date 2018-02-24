#!/usr/bin/env sh
# Emulator
for f in emulator/*.cpp; do
    b=$(basename $f)
    gcov -n -o "build/emulator/CMakeFiles/Chip8_Emulator.dir/$b.gcno" $f > /dev/null
done
# Assembler
for f in assembler/*.cpp; do
    b=$(basename $f)
    gcov -n -o "build/assembler/CMakeFiles/Chip8_Assembler.dir/$b.gcno" $f > /dev/null
done
# Disassembler
for f in disassembler/*.cpp; do
    b=$(basename $f)
    gcov -n -o "build/disassembler/CMakeFiles/Chip8_Disassembler.dir/$b.gcno" $f > /dev/null
done
