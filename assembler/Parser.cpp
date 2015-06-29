#include <cctype>

#include "Parser.h"
#include "Utilities.h"

Parser::Parser(string in_fn) {
    // Opens and reads from file to buffer
    FILE *f = fopen(in_fn.c_str(), "rb");

    if (f == NULL) {
        // ERROR
    }

    fseek(f, 0, SEEK_END);
    unsigned int file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Initialize buffer
    buffer = new char[file_size];
    for (int i=0; i<file_size; i++)
        buffer = 0;

    fread(buffer, 1, file_size, f);

    fclose(f);

    // Other intializations
    Parsed = "";
    ErrorMesg = "";
    pos = 0;
}

Parser::token Parser::getNextToken() {
    // Check for whitespace
    while (buffer[pos] == ' ' || buffer[pos] == '\t')
        pos++;

    if (isalpha(buffer[pos])) {
        // Check for a mnemonic or a label
        Parsed = buffer[pos];
        pos++;

        while (buffer[pos] != ':' && buffer[pos] != ' ' && buffer[pos] != '\n') {
            Parsed += buffer[pos];
            pos++;
        }

        if (buffer[pos] == ':') {
            // If it's a label, skip the colon and return
            pos++;
            return label_token;
        }
        else if (buffer[pos] == ' ') {
            // It should be a mnemonic
            return mnemonic_token;
        }
    }
    else if (buffer[pos] == 'v') {
        // Check if it names a valid register
        Parsed = "";
        pos++;
        if (isHexDigit(buffer[pos])) {
            Parsed = buffer[pos];
            pos++;
            return reg_token;
        }
        pos++;
    }
    else if (buffer[pos] == '0') {
        // Check if it is a hex number
        Parsed = "";
        pos++;
        if (buffer[pos] == 'x' || buffer[pos] == 'X') {
            pos++;
            while (isHexDigit(buffer[pos])) {
                Parsed += buffer[pos];
                pos++;
            }
        }
        else {
            pos++;
            ErrorMesg = "Expected 'x' after '0'";
        }
        pos++;

        // If there is nothing behind 0x, error out
        if (Parsed.length() >= 1) {
            return hexnum_token;
        }
        else {
            ErrorMesg = "Expected hexadecimal numbers after '0x'";
        }
    }
    else if (buffer[pos] == ',') {
        pos++;
        return comma_token;
    }
    else if (buffer[pos] == '\n') {
        pos++;
        return nl_token;
    }
    else if (buffer[pos] == EOF) {
        // End of file reached; abort
        // Don't increment counter
        return eof_token;
    }

    return error_state;
}
