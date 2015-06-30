#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "Chip_8.h"

Chip_8::Chip_8() {
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

void Chip_8::pushRegisters(unsigned char x) {
    for (unsigned char i=0; i<=x; i++) {
        memory[I+i] = V[i];
    }
}

void Chip_8::popRegisters(unsigned char x) {
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

void Chip_8::updateTimers() {
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
    switch (key) {
        case Keyboard::Num1:
            keys[1] = pressed;
            break;
        case Keyboard::Num2:
            keys[2] = pressed;
            break;
        case Keyboard::Num3:
            keys[3] = pressed;
            break;
        case Keyboard::Num4:
            keys[0xc] = pressed;
            break;
        case Keyboard::Q:
            keys[4] = pressed;
            break;
        case Keyboard::W:
            keys[5] = pressed;
            break;
        case Keyboard::E:
            keys[6] = pressed;
            break;
        case Keyboard::R:
            keys[0xd] = pressed;
            break;
        case Keyboard::A:
            keys[7] = pressed;
            break;
        case Keyboard::S:
            keys[8] = pressed;
            break;
        case Keyboard::D:
            keys[9] = pressed;
            break;
        case Keyboard::F:
            keys[0xe] = pressed;
            break;
        case Keyboard::Z:
            keys[0xa] = pressed;
            break;
        case Keyboard::X:
            keys[0] = pressed;
            break;
        case Keyboard::C:
            keys[0xb] = pressed;
            break;
        case Keyboard::V:
            keys[0xf] = pressed;
            break;
        default:
            break;
    }

    if (pressed && waitForKey) {
        waitForKey = false;
        keys[key_store] = true;
        pc += 2;
    }
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

    // Debug
    //printf("%04x: %x%x\n", pc, code[0], code[1]);

    switch (hinib) {
        case 0x0:
            switch (code[1]) {
                case 0xe0:
                    // 00E0: Clears the screen
                    clearScreen();
                    break;
                case 0xee:
                    // 00EE: Returns from subroutine
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
            pc = ((code[0]&0xf) << 8) | code[1];
            break;
        case 0x2:
            // Calls subroutine at address
            stack[sp++] = pc;
            stack[sp] = ((code[0]&0xf) << 8) | code[1];
            pc = stack[sp];
            break;
        case 0x3:
            // Skips next instruction if VX == NN
            pc += 2 + (V[code[0]&0xf] == code[1]? 2: 0);
            break;
        case 0x4:
            // Skips next instruction if VX != NN
            pc += 2 + (V[code[0]&0xf] != code[1]? 2: 0);
            break;
        case 0x5:
            // Skips next instruction if VX == VY
            pc += 2 + (V[code[0]&0xf] == V[code[1]>>4]? 2: 0);
            break;
        case 0x6:
            // Set VX to NN
            V[code[0]&0xf] = code[1];
            pc += 2;
            break;
        case 0x7:
            // Adds NN to VX
            V[code[0]&0xf] += code[1];
            pc += 2;
            break;
        case 0x8:
            switch (code[1]&0xf) {
                case 0x0:
                    // VX = VY
                    V[code[0]&0xf] = V[code[1]>>4];
                    break;
                case 0x1:
                    // VX |= VY
                    V[code[0]&0xf] |= V[code[1]>>4];
                    break;
                case 0x2:
                    // VX &= VY
                    V[code[0]&0xf] &= V[code[1]>>4];
                    break;
                case 0x3:
                    // VX xor= VY
                    V[code[0]&0xf] ^= V[code[1]>>4];
                    break;
                case 0x4:
                    {
                        // VX += VY, set VF = carry
                        unsigned int add = V[code[0]&0xf] + V[code[1]>>4];
                        V[code[0]&0xf] = (unsigned short) add&0x00ff;
                        V[0xf] = (unsigned short) add >> 8;
                        break;
                    }
                case 0x5:
                    // VX -= VY, set VF = 0 if borrow, 1 if no borrow
                    V[0xf] = V[code[0]&0xf] >= V[code[1]>>4]? 1: 0;
                    V[code[0]&0xf] -= V[code[1]>>4];
                    break;
                case 0x6:
                    // Set VF = least significant bit VX, VX >>= 1
                    V[0xf] = V[code[0]&0xf]&0x1;
                    V[code[0]&0xf] >>= 1;
                    break;
                case 0x7:
                    // VX = VY - VX, set VF = 0 if borrow, 1 if no borrow
                    V[0xf] = V[code[0]&0xf] <= V[code[1]>>4]? 1: 0;
                    V[code[0]&0xf] = V[code[1]>>4] - V[code[0]&0xf];
                    break;
                case 0xe:
                    // Set VF = most significant bit VX, VX <<= 1
                    V[0xf] = V[code[0]&0xf]>>7;
                    V[code[0]&0xf] <<= 1;
                    break;
            }
            pc += 2;
            break;
        case 0x9:
            // Skips next instruction if VX != VY
            pc += 2 + (V[code[0]&0xf] != V[code[1]>>4]? 2: 0);
            break;
        case 0xa:
            // Sets index register I to NNN
            I = ((code[0]&0xf) << 8) | code[1];
            pc += 2;
            break;
        case 0xb:
            // Jumps to address NNN+V0
            pc = ((code[0]&0xf) << 8) | code[1];
            pc += V[0];
            break;
        case 0xc:
            // Sets VX to random number masked by NN
            V[code[0]&0xf] = rand() & code[1];
            pc += 2;
            break;
        case 0xd:
            // Reads N bytes from memory, displays them at coords (VX, VY),
            // VF = XORed collision pixels (1 if collide, 0 if no collide
            V[0xf] = (unsigned short) displaySprite(V[code[0]&0xf], V[code[1]>>4], code[1]&0xf);
            pc += 2;
            drawf = true;
            break;
        case 0xe:
            switch (code[1]) {
                case 0x9e:
                    // Skip next instruction if key[VX] is pressed
                    pc += 2 + (keys[V[code[0]&0xf]]? 2: 0);
                    break;
                case 0xa1:
                    // Skip next instruction if key[VX] is not pressed
                    pc += 2 + (keys[V[code[0]&0xf]]? 0: 2);
                    break;
            }
            break;
        case 0xf:
            switch (code[1]) {
                case 0x07:
                    // VX = delay timer value
                    V[code[0]&0xf] = dt;
                    break;
                case 0x0a:
                    // Wait for keypress, store value of key in VX
                    waitForKey = true;
                    key_store = code[0]&0xf;
                    break;
                case 0x15:
                    // Delay timer value = VX
                    dt = V[code[0]&0xf];
                    break;
                case 0x18:
                    // Set sound timer = VX
                    st = V[code[0]&0xf];
                    break;
                case 0x1e:
                    // I += VX
                    I += V[code[0]&0xf];
                    break;
                case 0x29:
                    // I = location of sprite for digit VX
                    // Hex sprites are 8x5 bits
                    // They start at 0x00, so index them
                    I = V[code[0]&0xf]*5;
                    break;
                case 0x33:
                    // Store BCD representation of VX in memory location I, I+1, I+2
                    {
                        int regcode = code[0]&0xf;
                        unsigned char ones, tens, hundreds;
                        unsigned value = V[regcode];
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
                    pushRegisters(code[0]&0xf);
                    break;
                case 0x65:
                    // Reads from memory to registers V[0..X] starting from I
                    popRegisters(code[0]&0xf);
                    break;
            }
            pc += 2;
            break;
        default:
            // Skips the command
            pc += 2;
            status_txt = "Unknown opcode";
    }
}
