/*
 * This file is part of Chip8_Assembler.
 *
 * Chip8_Assembler is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Chip8_Assembler is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Chip8_Assembler.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstring>
#include <string>
#include <exception>
#include <iostream>

#include "CmdParser.h"

AssemblerCommandParser::AssemblerCommandParser(int numArgs, char** argv) {
	if (numArgs == 1) {
		AssemblerCommandException::throwHelp();
	}

	binaryFilename = "a.out";
	doOnePass = false;

    for (int i = 1; i < numArgs; i++) {
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0) {
            if (++i < numArgs) {
                binaryFilename = std::string(argv[i]);
            }
            else {
				AssemblerCommandException::throwError("Must have a filename after `-o'");
            }
        }
        else if (strcmp(argv[i], "-1p") == 0 || strcmp(argv[i], "--one-pass") == 0) {
            doOnePass = true;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			AssemblerCommandException::throwHelp();
        }
        else {
            sourceFilename = std::string(argv[i]);
        }
    }

	if (sourceFilename.empty()) {
		AssemblerCommandException::throwError("Must have a source file");
	}
}

AssemblerCommandException::AssemblerCommandException(
		AssemblerCommandExceptionType t,
		std::string huh) {
	type = t;
	issue = huh;
}

void AssemblerCommandException::throwHelp() {
	throw AssemblerCommandException(
			NEED_HELP,
			"");
}

void AssemblerCommandException::throwError(std::string huh) {
	throw AssemblerCommandException(
			UNRECOVERABLE_ERROR,
			huh);
}

void AssemblerCommandParser::printHelp() {
	std::cout << "Usage:\n"
        "./assembler [options] <source>\n\n"
        "Options:\n"
        "  -o <file>,\n"
        "  --out <file>             Place the output into <file>\n\n"
        "  -h, --help               Shows this help text\n\n"
        "  -1p, --one-pass          Makes 1 pass instead of 2, halving compilation\n"
        "                           time, but only use this option if you know you\n"
        "                           don't have any labels\n"
        "                           that are used before they are declared.\n";
}

const char* AssemblerCommandException::what() const throw() {
	return issue.c_str();
}
