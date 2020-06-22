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

#include <config.h>

#include "Parser.h"
#include "Generator.h"
#include "CmdParser.h"

using namespace std;

/* This assembler accepts only 1 source file */
int main(int argc, char **argv) {
	unique_ptr<AssemblerCommandParser> cmdParser;

	try {
		cmdParser = unique_ptr<AssemblerCommandParser>(new AssemblerCommandParser(argc, argv));

		unique_ptr<Generator> generator(new Generator(cmdParser.get()));

		generator->run();
		if (!generator->error) {
			generator->output(cmdParser->getBinaryFilename());
		} else {
			cerr << "Cannot generate binary - some errors exist.\n";
		}
	} catch (AssemblerCommandException& e) {
		if (e.isBadError()) {
			cerr << "Command line error: " << e.what() << endl;
		}
		if (e.isNeedHelp()) {
			cmdParser->printHelp();
		}
	} catch (ParseException& e) {
		cerr << "Parsing error: " << e.what() << endl;
	} catch (GeneratorException& e) {
		cerr << "Code generation error: " << e.what() << endl;
	}

	return 0;
}
