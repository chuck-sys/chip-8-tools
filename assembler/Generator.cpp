#include "Generator.h"

Generator::Generator(Parser *p) {
    parser = p;

    ind = 0;
    code = new unsigned char[CodeMaxSize];
}

void Generator::run() {
    /* Pass over the code 2 times
     *
     * The first pass would parse all the tokens
     * that aren't symbols (labels). It will store
     * all label names into a map.
     *
     * The second pass would find all the undefined
     * 'holes' in the generated code and fill them in.
     */
}
