#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

void emulateStep(unsigned char *buffer, unsigned int pc, bool clean, unsigned short offset=0x200) {
    // Run an opcode
    unsigned char *code = &buffer[pc];
    unsigned char hinib = code[0] >> 4;

    // Some useful variables
    unsigned nnn = ((code[0]&0xf) << 8) | code[1];
    unsigned nn = code[1];
    unsigned n = code[1] & 0xf;
    unsigned x = code[0] & 0xf;
    unsigned y = code[1] >> 4;

    if (!clean)
        printf("%04x: ", pc+offset);

    switch (hinib) {
        case 0x0:
            switch (nn) {
                case 0xe0:
                    // 00E0: Clears the screen
                    printf("CLS");
                    break;
                case 0xee:
                    // 00EE: Returns from subroutine
                    printf("RET");
                    break;
                default:
                    // If it is a piece of data, insert directly into output
                    printf("%02x%02x", code[0], code[1]);
                    break;
            }
            break;
        case 0x1:
            // Jumps to address
            printf("JP\t%04x", nnn);
            break;
        case 0x2:
            // Calls subroutine at address
            printf("CALL\t%04x", nnn);
            break;
        case 0x3:
            // Skips next instruction if VX == NN
            // SE
            printf("SE\tV%x, %02x", x, nn);
            break;
        case 0x4:
            // Skips next instruction if VX != NN
            // SNE
            printf("SNE\tV%x, %02x", x, nn);
            break;
        case 0x5:
            // Skips next instruction if VX == VY
            // SE
            printf("SE\tV%x, V%x", x, y);
            break;
        case 0x6:
            // Set VX to NN
            // LD
            printf("LD\tV%x, %02x", x, nn);
            break;
        case 0x7:
            // Adds NN to VX
            // ADD
            printf("ADD\tV%x, %02x", x, nn);
            break;
        case 0x8:
            switch (code[1]&0xf) {
                case 0x0:
                    // VX = VY
                    // LD
                    printf("LD\tV%x, V%x", x, y);
                    break;
                case 0x1:
                    // VX |= VY
                    // OR
                    printf("OR\tV%x, V%x", x, y);
                    break;
                case 0x2:
                    // VX &= VY
                    // AND
                    printf("AND\tV%x, V%x", x, y);
                    break;
                case 0x3:
                    // VX ^= VY
                    // XOR
                    printf("XOR\tV%x, V%x", x, y);
                    break;
                case 0x4:
                    // VX += VY, set VF = carry
                    // ADD
                    printf("ADD\tV%x, V%x", x, y);
                    break;
                case 0x5:
                    // VX -= VY, set VF = 0 if borrow, 1 if no borrow
                    // SUB
                    printf("SUB\tV%x, V%x", x, y);
                    break;
                case 0x6:
                    // Set VF = least significant bit VX, VX >>= 1
                    // SHR
                    printf("SHR\tV%x", x);
                    break;
                case 0x7:
                    // VX = VY - VX, set VF = 0 if borrow, 1 if no borrow
                    // SUBN
                    printf("SUBN\tV%x, V%x", x, y);
                    break;
                case 0xe:
                    // Set VF = most significant bit VX, VX <<= 1
                    // SHL
                    printf("SHL\tV%x", x);
                    break;
                default:
                    // If it is a piece of data, insert directly into output
                    printf("%02x%02x", code[0], code[1]);
                    break;
            }
            break;
        case 0x9:
            // Skips next instruction if VX != VY
            // SNE
            printf("SNE\tV%x, V%x", x, y);
            break;
        case 0xa:
            // Sets index register I to NNN
            // LD
            printf("LD\tI, %04x", nnn);
            break;
        case 0xb:
            // Jumps to address NNN+V0
            // JP
            printf("JP\tV0, %04x", nnn);
            break;
        case 0xc:
            // Sets VX to random number masked by NN
            // RND
            printf("RND\tV%x, %02x", x, nn);
            break;
        case 0xd:
            // Reads N bytes from memory, displays them at coords (VX, VY),
            // VF = XORed collision pixels (1 if collide, 0 if no collide)
            // DRW
            printf("DRW\tV%x, V%x, %02x", x, y, n);
            break;
        case 0xe:
            switch (code[1]) {
                case 0x9e:
                    // Skip next instruction if key[VX] is pressed
                    // SKP (Skip if Key Pressed)
                    printf("SKP\tV%x", x);
                    break;
                case 0xa1:
                    // Skip next instruction if key[VX] is not pressed
                    // SKNP
                    printf("SKNP\tV%x", x);
                    break;
                default:
                    // If it is a piece of data, insert directly into output
                    printf("%02x%02x", code[0], code[1]);
                    break;
            }
            break;
        case 0xf:
            switch (nn) {
                case 0x07:
                    // VX = delay timer value
                    // LD (Set Register to Delay timer)
                    printf("LD\tV%x, DT", x);
                    break;
                case 0x0a:
                    // Wait for keypress, store value of key in VX
                    // LD (Wait for Key Press)
                    printf("LD\tV%x, K", x);
                    break;
                case 0x15:
                    // Delay timer value = VX
                    // LD (Set Delay timer to Register)
                    printf("LD\tDT, V%x", x);
                    break;
                case 0x18:
                    // Set sound timer = VX
                    // LD (Set Sound timer to Register)
                    printf("LD\tST, V%x", x);
                    break;
                case 0x1e:
                    // I += VX
                    // ADD (Add I and Register)
                    printf("ADD\tI, V%x", x);
                    break;
                case 0x29:
                    // I = location of sprite for digit VX
                    // LDI (SET Hex)
                    printf("LDI\tV%x", x);
                    break;
                case 0x33:
                    // Store BCD representation of VX in memory location I, I+1, I+2
                    // BCD
                    printf("BCD\tV%x", x);
                    break;
                case 0x55:
                    // Push all registers V[0..X] to location I
                    // PUSH
                    printf("PUSH\tV%x", x);
                    break;
                case 0x65:
                    // Reads from memory to registers V[0..X] starting from I
                    // POP
                    printf("POP\tV%x", x);
                    break;
                default:
                    // If it is a piece of data, insert directly into output
                    printf("%02x%02x", code[0], code[1]);
                    break;
            }
            break;
        default:
            // Skips the command
            printf("???");
            break;
    }
    printf("\n");
}

int main(int argc, char **argv) {
    // Arguments checking
    if (argc == 1) {
        cerr << "Error: Invalid number of arguments.\n"
            << "Usage: " << argv[0] << " <program_file>\n";
        return -1;
    }
    bool clean = false;
    bool padded = true;
    for (int i=0; i<argc; i++) {
        if (strcmp(argv[i], "-c") == 0)
            clean = true;
        if (strcmp(argv[i], "--clean") == 0)
            clean = true;
        if (strcmp(argv[i], "-n") == 0)
            padded = false;
        if (strcmp(argv[i], "--no-padding") == 0)
            padded = false;
    }

    // Programs normally begin at
    // memory location 512 and above, so
    // load the file in there
    FILE *f = fopen(argv[1], "rb");

    if (f == NULL) {
        // Error: No file found
        return false;
    }

    // Grab file size
    fseek(f, 0, SEEK_END);
    int file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buffer = new unsigned char[file_size];
    for (int i=0; i<file_size; i++)
        buffer[i] = 0;

    // Read file to buffer
    fread(buffer, 1, file_size, f);

    fclose(f);



    unsigned short offset = 0;
    if (padded)
        offset = 0x200;



    for (unsigned int pc=0; pc<file_size; pc+=2) {
        emulateStep(buffer, pc, clean, offset);
    }

    return 0;
}
