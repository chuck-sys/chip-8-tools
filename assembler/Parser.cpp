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

#include "Parser.h"
#include "Utilities.h"

using namespace std;

Parser::Parser(string in_fn) {
    // Opens and reads from file to buffer
    infile.open(in_fn, ios::binary);

    if (infile.bad()) {
        ErrorMesg = "Fatal Error: File does not exist!!";
        return;
    }

    current = infile.get();

    // Other intializations
    Parsed = "";
    ErrorMesg = "";
}

Parser::token Parser::getNextToken() {
    // Skip any whitespace (not including a newline)
    while (current == ' ' || current == '\t') {
        current = infile.get();
    }

    if (current == 'V') {
        // Check if it names a valid register
        Parsed = "";
        current = infile.get();
        if (isHexDigit(current)) {
            Parsed = current;
            current = infile.get();
            return reg_token;
        }
        else {
            ErrorMesg = "Invalid register";
            return error_state;
        }
    }
    else if (current == ';') {
        // Ignore any and all comments
        while (current != '\n') {
            current = infile.get();
        }
    }
    else if (current == 'I') {
        // It is the indexer!
        current = infile.get();
        return ireg_token;
    }
    else if (current == 'K') {
        // It's a key!
        current = infile.get();
        return key_token;
    }
    else if (current == 'D') {
        if (infile.get() == 'T') {
            current = infile.get();
            return dt_token;
        }
        else {
            infile.unget();
            infile.unget();
            current = infile.get();
        }
    }
    if (current == 'S') {
        if (infile.get() == 'T') {
            current = infile.get();
            return st_token;
        }
        else {
            infile.unget();
            infile.unget();
            current = infile.get();
        }
    }
    if (isHexDigit(current)) {
        // Check if it is a hex number
        Parsed = current;
        current = infile.get();
        while (isHexDigit(current)) {
            Parsed += current;
            current = infile.get();
        }
        return hexnum_token;
    }
    if (isalpha(current)) {
        // Check for a mnemonic or a label
        Parsed = current;
        current = infile.get();

        while (current != ' '  && current != ':' &&
               current != '\n' && current != '\t') {
            Parsed += current;
            current = infile.get();
        }

        if (current == ':') {
            // If it's a label, skip the colon and return
            current = infile.get();
            return label_token;
        }
        else {
            // It should be a mnemonic
            return mnemonic_token;
        }
    }
    else if (current == ',') {
        current = infile.get();
        return comma_token;
    }
    else if (current == '\n') {
        current = infile.get();
        return nl_token;
    }
    else if (infile.eof()) {
        // End of file reached; abort
        // Don't increment counter
        return eof_token;
    }

    ErrorMesg = to_string(current);
    return error_state;
}

void Parser::setPosition(int p) {
    infile.clear();
    infile.seekg(p, ios::beg);
    current=infile.get();
}

int Parser::getPosition() {
    return infile.tellg();
}
