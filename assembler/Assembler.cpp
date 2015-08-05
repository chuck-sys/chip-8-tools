#include <cstring>
#include <iostream>

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

    // Argument parsing
    for (int i=0; i<argc; i++) {
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0) {
            // If you have set the output to something (maybe)
            // Set the output
            if (++i<argc)
                binary_fn = string(argv[i]);
            else {
                cerr << "Error: Must have a filename after `-o'\n";
                return -1;
            }
            continue;
        }
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            // Gives some help
            cout << "Chip 8 Assembler by Cheuk Yin Ng" << endl
                << "Report all bugs to <https://github.com/cheukyin699/chip-8-tools/>."
                << "Usage:\n"
                << argv[0] << " [options] <source>" << endl << endl
                << "Options:\n"
                << "  -o <file>             Place the output into <file>\n";
            return 0;
        }

        // If there is nothing else, set the source filename
        source_fn = string(argv[i]);
    }

    // Check for a source file
    if (source_fn == "") {
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

    generator->run();
    if (!generator->error)
        generator->output(binary_fn);
    else
        cerr << "Cannot generate binary - some errors exist.\n";

    return 0;
}
