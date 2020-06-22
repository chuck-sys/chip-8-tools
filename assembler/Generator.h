/*
 * This file is part of Chip8_Emulator.
 *
 * Chip8_Emulator is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Chip8_Emulator is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Chip8_Emulator.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <map>
#include <string>
#include <exception>

#include "CmdParser.h"
#include "Parser.h"

using namespace std;

class Generator {
private:
    Parser* parser;

    map<string, unsigned> symbols_map;

    unsigned char* code;
    unsigned ind;
    unsigned lineno;

    /* Handles incrementing the indexer too */
    void genForOpcode();

    void handlePError();

    inline bool isInMap(string);

    string lnToString();

public:
    static const unsigned CodeMaxSize = 2500;
    int passes;
    bool error;

    Generator(AssemblerCommandParser*);
    ~Generator();

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

class GeneratorException : public exception {
private:
	string msg;

public:
	GeneratorException(string huh): msg(huh) {};

	virtual const char* what() const throw() {return msg.c_str();}
};

#endif
