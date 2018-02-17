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

#include <cmath>
#include <string>

#include "Utilities.h"

bool isHexDigit(char c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f'));
}

int stringToHex(string seq) {
    return stoul(seq, nullptr, 16);
}
