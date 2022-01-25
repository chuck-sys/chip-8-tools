/*
 * This file is part of Chip8_Emulator.
 *
 * Chip8_Emulator is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Chip8_Emulator is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Chip8_Emulator.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "assembler/lex.h"
#include "grammar.tab.h"

class CodeGenerator {
    public:
        CodeGenerator() {};
        ~CodeGenerator() { delete parser; delete lexer; }

        void parse(std::string);
        void writeCodeToFile(std::string);

        void insertLabel(std::string);
        void insertInstruction(instruction_t);

    private:
        unsigned ind = 0x200;   // all programs start at 0x200
        std::vector<instruction_t> code;
        std::map<std::string, unsigned> symbolsMap;
        lex::Parser* parser = nullptr;
        lex::Lexer* lexer = nullptr;

        void toBytecode(instruction_t, char*);
};

#endif
