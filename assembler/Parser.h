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
    int file_size;
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
    void setPosition(int p) {infile.clear();infile.seekg(p, ios::beg);current=infile.get();}
    int getPosition() {return infile.tellg();}
};

#endif
