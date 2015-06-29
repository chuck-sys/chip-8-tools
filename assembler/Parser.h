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
    int pos;

public:
    enum token {
        error_state,
        eof_token,
        label_token,
        reg_token,
        hexnum_token,
        comma_token,
        mnemonic_token,
        nl_token
    };

    string Parsed;
    string ErrorMesg;

    Parser(string in_fn);

    token getNextToken();
    void setPosition(int p) {pos = p;}
    int getPosition() {return pos;}
};

#endif
