#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <map>
#include <string>

#include "Parser.h"

using namespace std;

class Generator {
private:
    Parser *parser;

    map<string, unsigned int> symbols_map;

    unsigned char *code;
    unsigned int ind;

public:
    static const unsigned int CodeMaxSize = 2048;

    Generator(Parser*);

    void run();
};

#endif
