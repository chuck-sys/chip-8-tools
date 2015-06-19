#ifndef PARSER_H_
#define PARSER_H_

#include <string>

using namespace std;

/* This is the parser class
 * Use this to parse the file for ease of code generation
 */
class Parser {
    private:
        char *buffer;

    public:
        Parser(string in_fn);
};

#endif
