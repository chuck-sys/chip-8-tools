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

#ifndef PARSER_H_
#define PARSER_H_

#include <fstream>
#include <string>

using namespace std;

/* This is the parser class
 * Use this to parse the file for ease of code generation
 */
class Parser {
private:
    char current;
    ifstream infile;

public:
    enum token {
        error_state,
        eof_token,
        label_token,
        reg_token,
        ireg_token,
        dt_token,
        st_token,
        key_token,
        hexnum_token,
        comma_token,
        mnemonic_token,
        nl_token
    };

    string Parsed;
    string ErrorMesg;

    Parser(string in_fn);
    ~Parser() {infile.close();}

    token getNextToken();
    void setPosition(int);
    int getPosition();
};

#endif
