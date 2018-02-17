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

#ifndef CHIP_8_H_
#define CHIP_8_H_

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

using namespace std;
using namespace sf;

class Chip_8 {
    private:
        static const int Width = 64;
        static const int Height = 32;
        static const int Scale = 8;

        unsigned char *memory;          // 4K memory
        unsigned char *display;         // Chip 8 graphics (B&W) 64x32px
        unsigned char V[16];            // 16 registers (V0 to VF)
        unsigned char key_store;        // Where do we store the key?
        unsigned short I;               // 16-bit address register
        unsigned short pc;              // 16-bit program counter
        unsigned short stack[16];       // 16 levels of stack
        unsigned short sp;              // Stack pointer
        bool keys[16];                  // Hex based keypad with 16 keys
        bool waitForKey;

        /* Loads the game file into memory */
        bool loadGame(const char *filename);

        /* Loads the hex sprites (0-F) into memory */
        void loadHexSprites();

        /* Clears the screen */
        void clearScreen();

        /* Displays n byte sprite at (x, y) from memory location I
         * Sets V[f] by itself (self contained)
         *
         * NOTE: bits are XORed to the screen
         */
        void displaySprite(unsigned char x, unsigned char y, unsigned char n_bytes);

        void updateTimers();
        void initKeyLookups();

        /* Pushes/Pops back all registers from/to V[0..x] starting
         * from memory position I
         */
        void pushRegisters(unsigned char x);
        void popRegisters(unsigned char x);

    public:
        unsigned dt;                // Delay timer register
        unsigned st;                // Sound timer register
        bool drawf;                 // Do we need to draw anything?
        bool error;
        string error_txt;

        Chip_8();
        ~Chip_8();

        void initialize(const char *filename);
        void handleKey(const Keyboard::Key key, bool pressed);
        void emulateStep();
        void drawDisplay(RenderWindow *w, Color bg, Color fg);
};

static unordered_map<int, int> key_lookup = unordered_map<int, int>();

#endif
