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
#include "Utilities.h"

Generator::Generator(Parser* p) {
    parser = p;

    passes = 2;
    error = false;
    ind = 0;
    lineno = 1;
    code = new unsigned char[CodeMaxSize];
}

Generator::~Generator() {
    delete parser;
    delete[] code;
}

inline bool Generator::isInMap(string k) {
    return (symbols_map.find(k) != symbols_map.end());
}

string Generator::lnToString() {
    string ret = "Line " + to_string(lineno) + ": ";
    return ret;
}

inline void Generator::handlePError() {
    cerr << lnToString() << parser->ErrorMesg << endl;
    error = true;
}

void Generator::genForOpcode() {
    // Try all 3X opcodes mnemonics
    // Go with the right ones (the ones on the site, not the
    // ones that I've created)
    string mnemonic = parser->Parsed;


    if (mnemonic == "CLS") {
        code[ind] = 0x00;
        code[ind+1] = 0xe0;
    }
    else if (mnemonic == "RET") {
        code[ind] = 0x00;
        code[ind+1] = 0xee;
    }
    else if (mnemonic == "JP") {
        Parser::token t = parser->getNextToken();
        if (t == Parser::hexnum_token) {
            int addr = stringToHex(parser->Parsed);

            code[ind] = 0x10 | addr >> 8;
            code[ind+1] = addr & 0xff;
        }
        else if (t == Parser::reg_token) {
            if (parser->Parsed != "0") {
                cerr << lnToString() << "Warning: JP doesn't work with registers other than `V0'. Defaulting to `V0'.\n";
            }
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::hexnum_token) {
                    int addr = stringToHex(parser->Parsed);

                    code[ind] = 0xb0 | (addr >> 8);
                    code[ind+1] = addr & 0xff;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a number or register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (t == Parser::mnemonic_token) {
            // Its actually a label in context, but whatever, right?
            string l = parser->Parsed;
            if (isInMap(l)) {
                int addr = symbols_map[l];

                code[ind] = 0x10 | addr >> 8;
                code[ind+1] = addr & 0xff;
            }
        }
        else if (t == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "Expected address or label\n";
            error = true;
        }
    }
    else if (mnemonic == "CALL") {
        Parser::token t = parser->getNextToken();
        if (t == Parser::hexnum_token) {
            int addr = stringToHex(parser->Parsed);

            code[ind] = 0x20 | addr >> 8;
            code[ind+1] = addr & 0xff;
        }
        else if (t == Parser::mnemonic_token) {
            // Its actually a label in context, but whatever, right?
            string l = parser->Parsed;
            if (isInMap(l)) {
                int addr = symbols_map[l];

                code[ind] = 0x20 | addr >> 8;
                code[ind+1] = addr & 0xff;
            }
        }
        else if (t == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "Expected address or label\n";
            error = true;
        }
    }
    else if (mnemonic == "SE") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::hexnum_token) {
                    unsigned char kk = stringToHex(parser->Parsed);

                    code[ind] = 0x30 | (v & 0xf);
                    code[ind+1] = kk;
                }
                else if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);

                    code[ind] = 0x50 | (v & 0xf);
                    code[ind+1] = (v2&0xf) << 4;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a number or register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "SNE") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::hexnum_token) {
                    unsigned char kk = stringToHex(parser->Parsed);

                    code[ind] = 0x40 | (v & 0xf);
                    code[ind+1] = kk;
                }
                else if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);

                    code[ind] = 0x90 | (v & 0xf);
                    code[ind+1] = (v2 & 0xf) << 4;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a number or register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "LD") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::hexnum_token) {
                    unsigned char kk = stringToHex(parser->Parsed);

                    code[ind] = 0x60 | (v&0xf);
                    code[ind+1] = kk;
                }
                else if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);

                    code[ind] = 0x80 | (v&0xf);
                    code[ind+1] = (v2&0xf) << 4;
                }
                else if (a2 == Parser::dt_token) {
                    code[ind] = 0xf0 | (v&0xf);
                    code[ind+1] = 0x7;
                }
                else if (a2 == Parser::key_token) {
                    code[ind] = 0xf0 | (v&0xf);
                    code[ind+1] = 0xa;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a number or register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::ireg_token) {
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token t = parser->getNextToken();
                if (t == Parser::hexnum_token) {
                    int addr = stringToHex(parser->Parsed);

                    code[ind] = 0xa0 | addr >> 8;
                    code[ind+1] = addr & 0xff;
                }
                else if (t == Parser::mnemonic_token) {
                    // Its actually a label in context, but whatever, right?
                    string l = parser->Parsed;
                    if (isInMap(l)) {
                        int addr = symbols_map[l];

                        code[ind] = 0xa0 | addr >> 8;
                        code[ind+1] = addr & 0xff;
                    }
                }
                else if (t == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Expected address or label\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::dt_token) {
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token t = parser->getNextToken();
                if (t == Parser::reg_token) {
                    unsigned char v = stringToHex(parser->Parsed);

                    code[ind] = 0xf0 | (v&0xf);
                    code[ind+1] = 0x15;
                }
                else if (t == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Expected address or label\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::st_token) {
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token t = parser->getNextToken();
                if (t == Parser::reg_token) {
                    unsigned char v = stringToHex(parser->Parsed);

                    code[ind] = 0xf0 | (v&0xf);
                    code[ind+1] = 0x18;
                }
                else if (t == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Expected address or label\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "OR") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);

                    code[ind] = 0x80 | (v & 0xf);
                    code[ind+1] = ((v2&0xf) << 4) | 1;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "AND") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);

                    code[ind] = 0x80 | (v & 0xf);
                    code[ind+1] = ((v2&0xf) << 4) | 2;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "XOR") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);

                    code[ind] = 0x80 | (v & 0xf);
                    code[ind+1] = ((v2&0xf) << 4) | 3;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "ADD") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::hexnum_token) {
                    unsigned char kk = stringToHex(parser->Parsed);

                    code[ind] = 0x70 | (v & 0xf);
                    code[ind+1] = kk;
                }
                else if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);

                    code[ind] = 0x80 | (v & 0xf);
                    code[ind+1] = ((v2 & 0xf) << 4) | 4;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a number or register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::ireg_token) {
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token t = parser->getNextToken();
                if (t == Parser::reg_token) {
                    unsigned char v = stringToHex(parser->Parsed);

                    code[ind] = 0xf0 | (v & 0xf);
                    code[ind+1] = 0x1e;
                }
                else if (t == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Expected second argument to be a register\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "SUB") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);

                    code[ind] = 0x80 | (v & 0xf);
                    code[ind+1] = ((v2 & 0xf) << 4) | 5;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "SUBN") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);

                    code[ind] = 0x80 | (v & 0xf);
                    code[ind+1] = ((v2 & 0xf) << 4) | 7;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "SHR") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);

            code[ind] = 0x80 | (v & 0xf);
            code[ind+1] = 0x6;
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "SHL") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);

            code[ind] = 0x80 | (v & 0xf);
            code[ind+1] = 0xe;
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "SKP") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);

            code[ind] = 0xe0 | (v & 0xf);
            code[ind+1] = 0x9e;
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "SKNP") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);

            code[ind] = 0xe0 | (v & 0xf);
            code[ind+1] = 0xa1;
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "LDI") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);

            code[ind] = 0xf0 | (v & 0xf);
            code[ind+1] = 0x29;
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "BCD") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);

            code[ind] = 0xf0 | (v & 0xf);
            code[ind+1] = 0x33;
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "PUSH") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);

            code[ind] = 0xf0 | (v & 0xf);
            code[ind+1] = 0x55;
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "POP") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);

            code[ind] = 0xf0 | (v & 0xf);
            code[ind+1] = 0x65;
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "RND") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::hexnum_token) {
                    unsigned char kk = stringToHex(parser->Parsed);

                    code[ind] = 0xc0 | (v & 0xf);
                    code[ind+1] = kk;
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a number or register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else if (mnemonic == "DRW") {
        Parser::token a1 = parser->getNextToken();
        if (a1 == Parser::reg_token) {
            unsigned char v = stringToHex(parser->Parsed);
            Parser::token comma = parser->getNextToken();
            if (comma == Parser::comma_token) {
                Parser::token a2 = parser->getNextToken();
                if (a2 == Parser::reg_token) {
                    unsigned char v2 = stringToHex(parser->Parsed);
                    comma = parser->getNextToken();
                    if (comma == Parser::comma_token) {
                        Parser::token nib = parser->getNextToken();
                        if (nib == Parser::hexnum_token) {
                            unsigned char n = stringToHex(parser->Parsed);

                            code[ind] = 0xd0 | (v & 0xf);
                            code[ind+1] = (v2 << 4) | (n & 0xf);
                        }
                        else if (a2 == Parser::error_state) {
                            handlePError();
                        }
                        else {
                            cerr << lnToString() << "Third argument expected to be a number.\n";
                            error = true;
                        }
                    }
                    else if (comma == Parser::error_state) {
                        handlePError();
                    }
                    else {
                        cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                        error = true;
                    }
                }
                else if (a2 == Parser::error_state) {
                    handlePError();
                }
                else {
                    cerr << lnToString() << "Second argument expected to be a register.\n";
                    error = true;
                }
            }
            else if (comma == Parser::error_state) {
                handlePError();
            }
            else {
                cerr << lnToString() << "Expected comma (`,') between arguments.\n";
                error = true;
            }
        }
        else if (a1 == Parser::error_state) {
            handlePError();
        }
        else {
            cerr << lnToString() << "First argument expected to be a register.\n";
            error = true;
        }
    }
    else {
        cerr << lnToString() << "Generator: Unknown mnemonic `" << mnemonic << "'\n";
        error = true;
    }
    ind += 2;
}

void Generator::run() {
    for (int pass = 0; pass < passes; pass++) {
        // Reset everything
        parser->setPosition(0);
        ind = 0;

        for (Parser::token t = parser->getNextToken();
                t != Parser::eof_token;
                t = parser->getNextToken()) {
            switch (t) {
                case Parser::error_state:
                    // Print the error, along with the line it occured on
                    handlePError();
                    break;
                case Parser::label_token:
                    // Record where it lay
                    symbols_map[parser->Parsed] = ind + 0x200;
                    break;
                case Parser::reg_token:
                    break;
                case Parser::hexnum_token:
                    {
                        // In the case of any numbers, insert them
                        // directly into the code
                        int data = stringToHex(parser->Parsed);
                        code[ind] = data >> 8;
                        code[ind+1] = data & 0xff;
                        ind += 2;
                        break;
                    }
                case Parser::comma_token:
                    break;
                case Parser::mnemonic_token:
                    // The lazy man's way out
                    genForOpcode();
                    break;
                case Parser::nl_token:
                    lineno++;
                    break;
                default:
                    break;
            }
        }
    }
}

void Generator::output(string fn) {
    ofstream of(fn, ios::binary);
    if (of.is_open()) {
        of.write(reinterpret_cast<char*>(code), ind);
    } else {
        cerr << "Error writing to file '" << fn << "'" << endl;
    }
    of.close();
}
