#include "Parser.h"

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
}
