#ifndef CHIP_8_H_
#define CHIP_8_H_

#include <SFML/Graphics.hpp>

#include <string>
#include <random>

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

        default_random_engine generator;
        uniform_int_distribution<unsigned short> distribution;

        /* Loads the game file into memory */
        bool loadGame(const char *filename);

        /* Loads the hex sprites (0-F) into memory */
        void loadHexSprites();

        /* Clears the screen */
        void clearScreen();

        /* Displays n byte sprite at (x, y) from memory location I
         * Returns any collisions
         *
         * NOTE: bits are XORed to the screen
         */
        bool displaySprite(unsigned char x, unsigned char y, unsigned char n_bytes);

        void updateTimers();

        /* Pushes/Pops back all registers from/to V[0..x] starting
         * from memory position I
         */
        void pushRegisters(unsigned char x);
        void popRegisters(unsigned char x);

    public:
        unsigned int dt;            // Delay timer register
        unsigned int st;            // Sound timer register
        bool drawf;                 // Do we need to draw anything?
        bool error;
        string error_txt;
        string status_txt;

        Chip_8();

        void initialize(const char *filename);
        void handleKeyPress(const Keyboard::Key key);
        void handleKeyUp(const Keyboard::Key key);
        void emulateStep();
        void drawDisplay(RenderWindow *w, Color bg, Color fg);
};

#endif
