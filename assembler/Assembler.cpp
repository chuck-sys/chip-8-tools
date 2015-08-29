#include <cstring>
#include <iostream>

#include <config.h>

#include "Parser.h"
#include "Generator.h"

using namespace std;

/* This assembler accepts only 1 source file */
int main(int argc, char **argv) {
    if (argc == 1) {
        cerr << "Error: Must have a filename\n";
        return -1;
    }

    string source_fn = "", binary_fn = "a.out";
    bool one_pass = false;

    // Argument parsing
    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0) {
            // If you have set the output to something (maybe)
            // Set the output
            if (++i<argc)
                binary_fn = string(argv[i]);
            else {
                cerr << "Error: Must have a filename after `-o'\n";
                return -1;
            }
        }
        else if (strcmp(argv[i], "-1p") == 0 || strcmp(argv[i], "--one-pass") == 0) {
            one_pass = true;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            // Gives some help
            cout << "Chip 8 Assembler " VERSION " by Cheuk Yin Ng\n"
                "Report all bugs to <" REP_ADDR ">.\n\n"
                "Usage:\n"
                << argv[0] << " [options] <source>\n\n"
                "Options:\n"
                "  -o <file>\n"
                "  --out <file>             Place the output into <file>\n\n"
                "  -h, --help               Shows this help text\n\n"
                "  -1p, --one-pass          Makes 1 pass instead of 2, halving compilation time, but\n"
                "                           only use this option if you know you don't have any labels\n"
                "                           that are used before they are declared.\n";
            return 0;
        }
        else {
            // If there is nothing else, set the source filename
            source_fn = string(argv[i]);
        }
    }

    // Check for a source file
    if (source_fn.empty()) {
        cerr << "Error: Must have a source file\n";
        return -1;
    }

    // Start parsing and generating the code
    Parser *parser = new Parser(source_fn);

    // Error checking before the storm
    if (parser->ErrorMesg != "") {
        cerr << "[Parser] " << parser->ErrorMesg << endl;
        return 0;
    }

    Generator *generator = new Generator(parser);

    // Sets the arguments
    if (one_pass)
        generator->passes = 1;

    generator->run();
    if (!generator->error)
        generator->output(binary_fn);
    else
        cerr << "Cannot generate binary - some errors exist.\n";

    return 0;
}
