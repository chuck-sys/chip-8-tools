#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <map>
#include <string>

#include "Parser.h"

using namespace std;

class Generator {
private:
    Parser *parser;

    // symbols_map["string"] = address
    map<string, unsigned int> symbols_map;

    unsigned char *code;
    unsigned int ind;
    unsigned int lineno;

    /* Handles incrementing the indexer too */
    void genForOpcode();

    void handlePError();

    inline bool isInMap(string);

    string lnToString();

public:
    static const unsigned int CodeMaxSize = 2500;
    int passes;
    bool error;

    Generator(Parser*);

    /* Pass over the code n times
     *
     * The first pass would parse all the tokens
     * that aren't symbols (labels). It will store
     * all label names into a map.
     *
     * The second pass would find all the undefined
     * 'holes' in the generated code and fill them in.
     */
    void run();

    /* (Should only be executed after you have finished
     * generating the code)
     *
     * Outputs code to specified output file
     */
    void output(string);
};

#endif
