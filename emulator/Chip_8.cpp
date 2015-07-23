#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "Chip_8.h"

Chip_8::Chip_8() {
    // Key lookups
    initKeyLookups();
    // The memory 4K
    memory = new unsigned char[4096];
    for (int i=0; i<4096; i++) {
        memory[i] = 0;
    }
    // Programs begin at 512
    pc = 512;
    // Display
    display = new unsigned char[Width*Height];
    for (int i=0; i<Width*Height; i++)
        display[i] = 0;
    // Stack init
    for (int i=0; i<16; i++)
        stack[i] = 0;
    // Keys init
    for (int i=0; i<16; i++)
        keys[i] = false;
    // Registry init
    for (int i=0; i<16; i++)
        V[i] = 0;

    // Initialize EVERYTHING to 0
    I = 0;
    sp = 0;
    srand(time(NULL));
    key_store = 0;

    waitForKey = false;

    dt = 0;
    st = 0;
    drawf = true;
    error = false;
    error_txt = "";
}

void Chip_8::initKeyLookups() {
    key_lookup[Keyboard::Num1]  = 0x1;
    key_lookup[Keyboard::Num2]  = 0x2;
    key_lookup[Keyboard::Num3]  = 0x3;
    key_lookup[Keyboard::Num4]  = 0xc;
    key_lookup[Keyboard::Q]     = 0x4;
    key_lookup[Keyboard::W]     = 0x5;
    key_lookup[Keyboard::E]     = 0x6;
    key_lookup[Keyboard::R]     = 0xd;
    key_lookup[Keyboard::A]     = 0x7;
    key_lookup[Keyboard::S]     = 0x8;
    key_lookup[Keyboard::D]     = 0x9;
    key_lookup[Keyboard::F]     = 0xe;
    key_lookup[Keyboard::Z]     = 0xa;
    key_lookup[Keyboard::X]     = 0x0;
    key_lookup[Keyboard::C]     = 0xb;
    key_lookup[Keyboard::V]     = 0xf;
}

void Chip_8::loadHexSprites() {
    // 0
    memory[0x000] = 0xf0;
    memory[0x001] = 0x90;
    memory[0x002] = 0x90;
    memory[0x003] = 0x90;
    memory[0x004] = 0xf0;

    // 1
    memory[0x005] = 0x20;
    memory[0x006] = 0x60;
    memory[0x007] = 0x20;
    memory[0x008] = 0x20;
    memory[0x009] = 0x70;

    // 2
    memory[0x00a] = 0xf0;
    memory[0x00b] = 0x10;
    memory[0x00c] = 0xf0;
    memory[0x00d] = 0x80;
    memory[0x00e] = 0xf0;

    // 3
    memory[0x00f] = 0xf0;
    memory[0x010] = 0x10;
    memory[0x011] = 0xf0;
    memory[0x012] = 0x10;
    memory[0x013] = 0xf0;

    // 4
    memory[0x014] = 0x90;
    memory[0x015] = 0x90;
    memory[0x016] = 0xf0;
    memory[0x017] = 0x10;
    memory[0x018] = 0x10;

    // 5
    memory[0x019] = 0xf0;
    memory[0x01a] = 0x80;
    memory[0x01b] = 0xf0;
    memory[0x01c] = 0x10;
    memory[0x01d] = 0xf0;

    // 6
    memory[0x01e] = 0xf0;
    memory[0x01f] = 0x80;
    memory[0x020] = 0xf0;
    memory[0x021] = 0x90;
    memory[0x022] = 0xf0;

    // 7
    memory[0x023] = 0xf0;
    memory[0x024] = 0x10;
    memory[0x025] = 0x20;
    memory[0x026] = 0x40;
    memory[0x027] = 0x40;

    // 8
    memory[0x028] = 0xf0;
    memory[0x029] = 0x90;
    memory[0x02a] = 0xf0;
    memory[0x02b] = 0x90;
    memory[0x02c] = 0xf0;

    // 9
    memory[0x02d] = 0xf0;
    memory[0x02e] = 0x90;
    memory[0x02f] = 0xf0;
    memory[0x030] = 0x10;
    memory[0x031] = 0xf0;

    // A
    memory[0x032] = 0xf0;
    memory[0x033] = 0x90;
    memory[0x034] = 0xf0;
    memory[0x035] = 0x90;
    memory[0x036] = 0x90;

    // B
    memory[0x037] = 0xe0;
    memory[0x038] = 0x90;
    memory[0x039] = 0xe0;
    memory[0x03a] = 0x90;
    memory[0x03b] = 0xe0;

    // C
    memory[0x03c] = 0xf0;
    memory[0x03d] = 0x80;
    memory[0x03e] = 0x80;
    memory[0x03f] = 0x80;
    memory[0x040] = 0xf0;

    // D
    memory[0x041] = 0xe0;
    memory[0x042] = 0x90;
    memory[0x043] = 0x90;
    memory[0x044] = 0x90;
    memory[0x045] = 0xe0;

    // E
    memory[0x046] = 0xf0;
    memory[0x047] = 0x80;
    memory[0x048] = 0xf0;
    memory[0x049] = 0x80;
    memory[0x04a] = 0xf0;

    // F
    memory[0x04b] = 0xf0;
    memory[0x04c] = 0x80;
    memory[0x04d] = 0xf0;
    memory[0x04e] = 0x80;
    memory[0x04f] = 0x80;
}

inline void Chip_8::pushRegisters(unsigned char x) {
    for (unsigned char i=0; i<=x; i++) {
        memory[I+i] = V[i];
    }
}

inline void Chip_8::popRegisters(unsigned char x) {
    for (unsigned char i=0; i<=x; i++) {
        V[i] = memory[I+i];
    }
}

void Chip_8::clearScreen() {
    for (int i=0; i<Width*Height; i++)
        display[i] = 0;
    drawf = true;
}

bool Chip_8::displaySprite(unsigned char x, unsigned char y, unsigned char n_bytes) {
    V[0xF] = 0;
    for (int by = 0; by < n_bytes; by++) {
        unsigned char sprite = memory[I + by];
        for (int bx = 0; bx < 8; bx++) {
            if ((sprite & (0x80 >> bx)) != 0) {
                if (display[(x + bx) + ((y + by) * 64)] == 1) {
                    V[0xF] = 1;
                }
                display[(x + bx) + ((y + by) * 64)] ^= 1;
            }
        }
    }
}

bool Chip_8::loadGame(const char *filename) {
    // Programs normally begin at
    // memory location 512 and above, so
    // load the file in there
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        // Error: No file found
        return false;
    }

    unsigned char *buffer = memory + 512;

    // Grab file size
    fseek(f, 0, SEEK_END);
    int file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read file to buffer
    fread(buffer, 1, file_size, f);

    fclose(f);

    return true;
}

inline void Chip_8::updateTimers() {
    if (dt > 0)
        dt--;
    if (st > 0)
        st--;
}

void Chip_8::initialize(const char *filename) {
    bool success = loadGame(filename);
    if (!success) {
        // Set the error flag and error text
        error = true;
        error_txt = "Unable to load game";
    }

    loadHexSprites();
}

void Chip_8::handleKey(const Keyboard::Key key, bool pressed) {
    try {
        keys[key_lookup.at(key)] = pressed;

        if (pressed && waitForKey) {
            waitForKey = false;
            V[key_store] = key_lookup.at(key);
            pc += 2;
        }
    } catch (std::out_of_range &e) {}
}

void Chip_8::drawDisplay(RenderWindow *w, Color bg, Color fg) {
    // Draws every single pixel
    for (int y=0; y<Height; y++) {
        for (int x=0; x<Width; x++) {
            RectangleShape rs;
            rs.setPosition(x*Scale, y*Scale);
            rs.setSize(Vector2f(Scale, Scale));
            if (display[y*Width+x] == 1)
                rs.setFillColor(fg);
            else
                rs.setFillColor(bg);
            w->draw(rs);
        }
    }
    drawf = false;
}

void Chip_8::emulateStep() {
    // Update the timers
    updateTimers();

    // Run an opcode
    unsigned char *code = &memory[pc];
    unsigned char hinib = code[0] >> 4;

    // Some useful variables
    unsigned nnn = ((code[0]&0xf) << 8) | code[1];
    unsigned nn = code[1];
    unsigned n = code[1] & 0xf;
    unsigned x = code[0] & 0xf;
    unsigned y = code[1] >> 4;
    // Register aliases
    unsigned char &Vx = V[x], &Vy = V[y], &VF = V[0xf];

    switch (hinib) {
        case 0x0:
            switch (nn) {
                case 0xe0:
                    // 00E0: Clears the screen
                    // Tested: Good
                    clearScreen();
                    break;
                case 0xee:
                    // 00EE: Returns from subroutine
                    // Tested: Good
                    if (--sp < 0) {
                        error = true;
                        error_txt = "Error: No subroutine to return from\n";
                    }
                    pc = stack[sp];
                    break;
            }
            // Step pc
            pc += 2;
            break;
        case 0x1:
            // Jumps to address
            // Tested: Good
            pc = nnn;
            break;
        case 0x2:
            // Calls subroutine at address
            // Tested: Good
            stack[sp++] = pc;
            stack[sp] = nnn;
            pc = stack[sp];
            break;
        case 0x3:
            // Skips next instruction if VX == NN
            // Tested: Good
            pc += 2 + (Vx == nn? 2: 0);
            break;
        case 0x4:
            // Skips next instruction if VX != NN
            // Tested: Good
            pc += 2 + (Vx != nn? 2: 0);
            break;
        case 0x5:
            // Skips next instruction if VX == VY
            // Tested: Good
            pc += 2 + (Vx == Vy? 2: 0);
            break;
        case 0x6:
            // Set VX to NN
            // Tested: Good
            Vx = nn;
            pc += 2;
            break;
        case 0x7:
            // Adds NN to VX
            // Tested: Good
            Vx += nn;
            pc += 2;
            break;
        case 0x8:
            switch (n) {
                case 0x0:
                    // VX = VY
                    // Tested: Good
                    Vx = Vy;
                    break;
                case 0x1:
                    // VX |= VY
                    // Tested: Good
                    Vx |= Vy;
                    break;
                case 0x2:
                    // VX &= VY
                    // Tested: Good
                    Vx &= Vy;
                    break;
                case 0x3:
                    // VX xor= VY
                    // Tested: Good
                    Vx ^= Vy;
                    break;
                case 0x4:
                    {
                        // VX += VY, set VF = carry
                        // Tested: Good
                        unsigned int add = Vx + Vy;
                        Vx = (unsigned short) add;
                        VF = (unsigned short) add >> 16;
                        break;
                    }
                case 0x5:
                    // VX -= VY, set VF = 0 if borrow, 1 if no borrow
                    // Tested: Good
                    VF = Vx >= Vy? 1: 0;
                    Vx -= Vy;
                    break;
                case 0x6:
                    // Set VF = least significant bit VX, VX >>= 1
                    // Tested: Good
                    VF = Vx&0x1;
                    Vx >>= 1;
                    break;
                case 0x7:
                    // VX = VY - VX, set VF = 0 if borrow, 1 if no borrow
                    VF = Vx <= Vy? 1: 0;
                    Vx = Vy - Vx;
                    break;
                case 0xe:
                    // Set VF = most significant bit VX, VX <<= 1
                    // Tested: Good
                    VF = Vx>>7;
                    Vx <<= 1;
                    break;
            }
            pc += 2;
            break;
        case 0x9:
            // Skips next instruction if VX != VY
            // Tested: Good
            pc += 2 + (Vx != Vy? 2: 0);
            break;
        case 0xa:
            // Sets index register I to NNN
            // Tested: Good
            I = nnn;
            pc += 2;
            break;
        case 0xb:
            // Jumps to address NNN+V0
            // Tested: Good
            pc = nnn+V[0];
            break;
        case 0xc:
            // Sets VX to random number masked by NN
            // Tested: Good
            Vx = rand() & nn;
            pc += 2;
            break;
        case 0xd:
            // Reads N bytes from memory, displays them at coords (VX, VY),
            // VF = XORed collision pixels (1 if collide, 0 if no collide
            // Tested: Good
            VF = (unsigned short) displaySprite(Vx, Vy, n);
            pc += 2;
            drawf = true;
            break;
        case 0xe:
            switch (nn) {
                case 0x9e:
                    // Skip next instruction if key[VX] is pressed
                    // Tested: Good
                    pc += 2 + (keys[Vx]? 2: 0);
                    break;
                case 0xa1:
                    // Skip next instruction if key[VX] is not pressed
                    // Tested: Good
                    pc += 2 + (!keys[Vx]? 2: 0);
                    break;
            }
            break;
        case 0xf:
            switch (nn) {
                case 0x07:
                    // VX = delay timer value
                    // Tested: Good
                    Vx = dt;
                    break;
                case 0x0a:
                    // Wait for keypress, store value of key in VX
                    waitForKey = true;
                    key_store = x;
                    break;
                case 0x15:
                    // Delay timer value = VX
                    // Tested: Good
                    dt = Vx;
                    break;
                case 0x18:
                    // Set sound timer = VX
                    // Tested: Good
                    st = Vx;
                    break;
                case 0x1e:
                    // I += VX
                    // Tested: Good
                    I += Vx;
                    break;
                case 0x29:
                    // I = location of sprite for digit VX
                    // Hex sprites are 8x5 bits
                    // They start at 0x00, so index them
                    // Tested: Good
                    I = Vx*5;
                    break;
                case 0x33:
                    // Store BCD representation of VX in memory location I, I+1, I+2
                    // Tested: Good
                    {
                        unsigned char ones, tens, hundreds;
                        unsigned value = Vx;
                        ones = value % 10;
                        value /= 10;
                        tens = value % 10;
                        hundreds = value / 10;
                        memory[I] = hundreds;
                        memory[I+1] = tens;
                        memory[I+2] = ones;
                    }
                    break;
                case 0x55:
                    // Push all registers V[0..X] to location I
                    // Tested: Good
                    pushRegisters(x);
                    break;
                case 0x65:
                    // Reads from memory to registers V[0..X] starting from I
                    // Tested: Good
                    popRegisters(x);
                    break;
            }
            pc += 2;
            break;
        default:
            // Skips the command
            pc += 2;
            error = true;
            error_txt = "Unknown opcode";
    }
}
