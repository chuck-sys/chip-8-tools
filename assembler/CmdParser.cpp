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

asm_options_t* parseOptions(int argc, const char** argv) {
    auto ret = new asm_options_t;
    ret->out = "a.out";

	if (argc == 1) {
        goto needhelp;
	}

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0) {
            if (++i < argc) {
                ret->out = std::string(argv[i]);
            } else {
                goto needhelp;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            goto needhelp;
        } else {
            ret->src = std::string(argv[i]);
        }
    }

	if (ret->src.empty()) {
        goto needhelp;
	}

    return ret;

needhelp:
    showHelp(argv);
    delete ret;
    return nullptr;
}

void showHelp(const char** argv) {
	std::cout << "Usage:\n" << argv[0] <<
        " [options] <source>\n\n"
        "If you give it multiple source files, it only selects the latest one.\n\n"
        "Options:\n"
        "  -o <file>,\n"
        "  --out <file>             Place the output into <file>\n\n"
        "  -h, --help               Shows this help text\n";
}
