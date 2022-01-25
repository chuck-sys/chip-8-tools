/*
 * This file is part of Chip8_Assembler.
 *
 * Chip8_Assembler is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Chip8_Assembler is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Chip8_Assembler.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <iostream>

#include "Generator.h"
#include "CmdParser.h"

void CodeGenerator::parse(std::string filename) {
    if (lexer != nullptr) {
        delete lexer;
        delete parser;
    }

    auto iss = new std::ifstream(filename, std::ios::binary);
    if (!iss->good()) {
        throw std::runtime_error("Could not open file for reading.");
    }

    lexer = new lex::Lexer(iss);
    parser = new lex::Parser(*lexer, *this);

    if (parser->parse() != 0) {
        std::cerr << "Parse failed!" << std::endl;
    }
    delete iss;
}

void CodeGenerator::writeCodeToFile(std::string filename) {
    std::ofstream iss(filename, std::ios::binary);
    if (iss.is_open()) {
        char buffer[2];
        for (auto instr : code) {
            toBytecode(instr, buffer);
            iss << buffer[0] << buffer[1];
        }

        iss.close();
    } else {
        throw std::runtime_error("Could not open file for writing.");
    }
}

void CodeGenerator::insertLabel(std::string identifier) {
    symbolsMap[identifier] = ind;
}

void CodeGenerator::insertInstruction(instruction_t instr) {
    code.push_back(instr);
    ind += 2;
}

void CodeGenerator::toBytecode(instruction_t instr, char* output) {
    using token = lex::Parser::token;

#define SYMNNN(op) { \
    unsigned addr = symbolsMap[instr.label]; \
    output[0] = op | addr >> 8; \
    output[1] = addr & 0xff; \
}
#define NNN(op) { \
    unsigned addr = instr.arg1.literal; \
    output[0] = op | addr >> 8; \
    output[1] = addr & 0xff; \
}
#define XNN(op) { \
    output[0] = op | (instr.arg1.reg & 0xf); \
    output[1] = instr.arg2.literal & 0xff; \
}
#define XYC(op, c) { \
    output[0] = op | (instr.arg1.reg & 0xf); \
    output[1] = ((instr.arg2.reg & 0xf) << 4) | (c & 0xf); \
}
#define XYN(op) { \
    output[0] = op | (instr.arg1.reg & 0xf); \
    output[1] = ((instr.arg2.reg & 0xf) << 4) | (instr.arg3.literal & 0xf); \
}
#define XCC(op, c) { \
    output[0] = op | (instr.arg1.reg & 0xf); \
    output[1] = c & 0xff; \
}
#define DATA() { \
    output[0] = instr.arg1.literal >> 8; \
    output[1] = instr.arg1.literal & 0xff; \
}

    switch (instr.token) {
        case token::CLS:
            output[0] = 0x00;
            output[1] = 0xe0;
            break;
        case token::RET:
            output[0] = 0x00;
            output[1] = 0xee;
            break;
        case token::JP:
            SYMNNN(instr.arg1.token == token::REGISTER? 0xb0 : 0x10);
            break;
        case token::CALL:
            SYMNNN(0x20);
            break;
        case token::SE:
            if (instr.arg2.token == token::LITERAL) {
                XNN(0x30);
            } else if (instr.arg2.token == token::REGISTER) {
                XYC(0x50, 0);
            }
            break;
        case token::SNE:
            if (instr.arg2.token == token::LITERAL) {
                XNN(0x40);
            } else if (instr.arg2.token == token::REGISTER) {
                XYC(0x90, 0);
            }
            break;
        case token::LD:
            if (instr.arg1.token == token::REGISTER) {
                if (instr.arg2.token == token::LITERAL) {
                    XNN(0x60);
                } else if (instr.arg2.token == token::REGISTER) {
                    XYC(0x80, 0);
                } else if (instr.arg2.token == token::DT) {
                    XCC(0xf0, 0x07);
                } else if (instr.arg2.token == token::K) {
                    XCC(0xf0, 0x0a);
                }
            } else if (instr.arg1.token == token::IREG) {
                NNN(0xa0);
            } else if (instr.arg1.token == token::DT) {
                XCC(0xf0, 0x15);
            } else if (instr.arg1.token == token::ST) {
                XCC(0xf0, 0x18);
            }
            break;
        case token::OR:
            XYC(0x80, 1);
            break;
        case token::AND:
            XYC(0x80, 2);
            break;
        case token::XOR:
            XYC(0x80, 3);
            break;
        case token::ADD:
            if (instr.arg2.token == token::LITERAL) {
                XNN(0x70);
            } else if (instr.arg2.token == token::REGISTER) {
                XYC(0x80, 4);
            } else if (instr.arg1.token == token::IREG) {
                XCC(0xf0, 0x1e);
            }
            break;
        case token::SUB:
            XYC(0x80, 5);
            break;
        case token::SHR:
            XCC(0x80, 0x06);
            break;
        case token::SUBN:
            XYC(0x80, 7);
            break;
        case token::SHL:
            XCC(0x80, 0x0e);
            break;
        case token::RND:
            XNN(0xc0);
            break;
        case token::DRW:
            XYN(0xd0);
            break;
        case token::SKP:
            XCC(0xe0, 0x9e);
            break;
        case token::SKNP:
            XCC(0xe0, 0xa1);
            break;
        case token::BCD:
            XCC(0xf0, 0x33);
            break;
        case token::PUSH:
            XCC(0xf0, 0x55);
            break;
        case token::POP:
            XCC(0xf0, 0x65);
            break;
        case token::LITERAL:
            DATA();
            break;
        default:
            break;
    }
}
