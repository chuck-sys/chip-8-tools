#include <cctype>

#include "Parser.h"
#include "Utilities.h"

Parser::Parser(string in_fn) {
    // Opens and reads from file to buffer
    FILE *f = fopen(in_fn.c_str(), "rb");

    if (f == NULL) {
        // ERROR
        return;
    }

    fseek(f, 0, SEEK_END);
    unsigned int file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Initialize buffer
    buffer = new char[file_size];
    for (int i=0; i<file_size; i++)
        buffer[i] = 0;

    fread(buffer, 1, file_size, f);

    fclose(f);

    // Other intializations
    Parsed = "";
    ErrorMesg = "";
    pos = 0;
}

Parser::token Parser::getNextToken() {
    // Skip any whitespace (not including a newline)
    while (buffer[pos] == ' ' || buffer[pos] == '\t')
        pos++;

    if (buffer[pos] == 'V') {
        // Check if it names a valid register
        Parsed = "";
        pos++;
        if (isHexDigit(buffer[pos])) {
            Parsed = buffer[pos];
            pos++;
            return reg_token;
        }
        else {
            ErrorMesg = "Invalid register";
            return error_state;
        }
    }
    else if (buffer[pos] == ';') {
        // Ignore any and all comments
        while (buffer[pos] != '\n')
            pos++;
    }
    else if (buffer[pos] == 'I') {
        // It is the indexer!
        pos++;
        return ireg_token;
    }
    else if (buffer[pos] == 'K') {
        // It's a key!
        pos++;
        return key_token;
    }
    else if (buffer[pos] == 'D') {
        if (buffer[pos+1] == 'T') {
            pos += 2;
            return dt_token;
        }
    }
    if (buffer[pos] == 'S') {
        if (buffer[pos+1] == 'T') {
            pos += 2;
            return st_token;
        }
    }
    if (isHexDigit(buffer[pos])) {
        // Check if it is a hex number
        Parsed = buffer[pos];
        pos++;
        while (isHexDigit(buffer[pos])) {
            Parsed += buffer[pos];
            pos++;
        }
        return hexnum_token;
    }
    if (isalpha(buffer[pos])) {
        // Check for a mnemonic or a label
        Parsed = buffer[pos];
        pos++;

        while (buffer[pos] != ':' && buffer[pos] != ' ' && buffer[pos] != '\n' && buffer[pos] != '\t') {
            Parsed += buffer[pos];
            pos++;
        }

        if (buffer[pos] == ':') {
            // If it's a label, skip the colon and return
            pos++;
            return label_token;
        }
        else {
            // It should be a mnemonic
            return mnemonic_token;
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
    else if (pos >= file_size) {
        // End of file reached; abort
        // Don't increment counter
        return eof_token;
    }

    ErrorMesg = to_string(buffer[pos]);
    return error_state;
}
