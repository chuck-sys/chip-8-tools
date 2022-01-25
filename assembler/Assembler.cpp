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
#include <iostream>
#include <memory>

#include "Generator.h"
#include "CmdParser.h"

using namespace std;

/* This assembler accepts only 1 source file */
int main(int argc, const char **argv) {
    auto options = parseOptions(argc, argv);
    if (options == nullptr) {
        delete options;
        return 1;
    }

    unique_ptr<CodeGenerator> generator(new CodeGenerator());

    generator->parse(options->src);
    generator->writeCodeToFile(options->out);

    delete options;
	return 0;
}
