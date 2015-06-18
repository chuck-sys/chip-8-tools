#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

void emulateStep(unsigned char *buffer, unsigned int pc, bool clean, unsigned short offset=0x200) {
    // Run an opcode
    unsigned char *code = &buffer[pc];
    unsigned char hinib = code[0] >> 4;

    if (!clean)
        printf("%04x: ", pc+offset);

    switch (hinib) {
        case 0x0:
            switch (code[1]) {
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
            printf("JMP\t%04x", ((code[0]&0xf) << 8) | code[1]);
            break;
        case 0x2:
            // Calls subroutine at address
            printf("CALL\t%04x", ((code[0]&0xf) << 8) | code[1]);
            break;
        case 0x3:
            // Skips next instruction if VX == NN
            // SVE
            printf("SVE\tV[%x], %02x", code[0]&0xf, code[1]);
            break;
        case 0x4:
            // Skips next instruction if VX != NN
            // SVNE
            printf("SVNE\tV[%x], %02x", code[0]&0xf, code[1]);
            break;
        case 0x5:
            // Skips next instruction if VX == VY
            // SVVE
            printf("SVVE\tV[%x], V[%x]", code[0]&0xf, code[1]>>4);
            break;
        case 0x6:
            // Set VX to NN
            // MOV
            printf("MOV\tV[%x], %02x", code[0]&0xf, code[1]);
            break;
        case 0x7:
            // Adds NN to VX
            // ADD
            printf("ADD\tV[%x], %02x", code[0]&0xf, code[1]);
            break;
        case 0x8:
            switch (code[1]&0xf) {
                case 0x0:
                    // VX = VY
                    // MOVR
                    printf("MOVR\tV[%x], V[%x]", code[0]&0xf, code[1]>>4);
                    break;
                case 0x1:
                    // VX |= VY
                    // ORR
                    printf("ORR\tV[%x], V[%x]", code[0]&0xf, code[1]>>4);
                    break;
                case 0x2:
                    // VX &= VY
                    // ANR
                    printf("ANR\tV[%x], V[%x]", code[0]&0xf, code[1]>>4);
                    break;
                case 0x3:
                    // VX ^= VY
                    // XORR
                    printf("XORR\tV[%x], V[%x]", code[0]&0xf, code[1]>>4);
                    break;
                case 0x4:
                    // VX += VY, set VF = carry
                    // ADC
                    printf("ADC\tV[%x], V[%x]", code[0]&0xf, code[1]>>4);
                    break;
                case 0x5:
                    // VX -= VY, set VF = 0 if borrow, 1 if no borrow
                    // SBB
                    printf("SBB\tV[%x], V[%x]", code[0]&0xf, code[1]>>4);
                    break;
                case 0x6:
                    // Set VF = least significant bit VX, VX >>= 1
                    // SRS (Shift Right Save (bit))
                    printf("SRS\tV[%x]", code[0]&0xf);
                    break;
                case 0x7:
                    // VX = VY - VX, set VF = 0 if borrow, 1 if no borrow
                    // SRB (Sub Reverse Borrow)
                    printf("SRB\tV[%x], V[%x]", code[0]&0xf, code[1]>>4);
                    break;
                case 0xe:
                    // Set VF = most significant bit VX, VX <<= 1
                    // SLS (Shift Left Save (bit))
                    printf("SLS\tV[%x]", code[0]&0xf);
                    break;
                default:
                    // If it is a piece of data, insert directly into output
                    printf("%02x%02x", code[0], code[1]);
                    break;
            }
            break;
        case 0x9:
            // Skips next instruction if VX != VY
            // SVVNE
            printf("SVVNE\tV[%x], V[%x]", code[0]&0xf, code[1]>>4);
            break;
        case 0xa:
            // Sets index register I to NNN
            // SETI
            printf("SETI\t%04x", ((code[0]&0xf) << 8) | code[1]);
            break;
        case 0xb:
            // Jumps to address NNN+V0
            // JAR (Jump Add Register V[0]
            printf("JAR\t%04x", ((code[0]&0xf) << 8) | code[1]);
            break;
        case 0xc:
            // Sets VX to random number masked by NN
            // RNRM (RaNdom Register with Mask)
            printf("RNRM\tV[%x], %02x", code[0]&0xf, code[1]<<4);
            break;
        case 0xd:
            // Reads N bytes from memory, displays them at coords (VX, VY),
            // VF = XORed collision pixels (1 if collide, 0 if no collide)
            // DSP (Display SPrite)
            printf("DSP\tV[%x], V[%x], %02x", code[0]&0xf, code[1]>>4, code[1]%0xf);
            break;
        case 0xe:
            switch (code[1]) {
                case 0x9e:
                    // Skip next instruction if key[VX] is pressed
                    // SKP (Skip if Key Pressed)
                    printf("SKP\tK[%x]", code[0]&0xf);
                    break;
                case 0xa1:
                    // Skip next instruction if key[VX] is not pressed
                    // SKNP
                    printf("SKNP\tK[%x]", code[0]&0xf);
                    break;
                default:
                    // If it is a piece of data, insert directly into output
                    printf("%02x%02x", code[0], code[1]);
                    break;
            }
            break;
        case 0xf:
            switch (code[1]) {
                case 0x07:
                    // VX = delay timer value
                    // SRD (Set Register to Delay timer)
                    printf("SRD\tV[%x]", code[0]&0xf);
                    break;
                case 0x0a:
                    // Wait for keypress, store value of key in VX
                    // WKP (Wait for Key Press)
                    printf("WKP\tV[%x]", code[0]&0xf);
                    break;
                case 0x15:
                    // Delay timer value = VX
                    // SDR (Set Delay timer to Register)
                    printf("SDR\tV[%x]", code[0]&0xf);
                    break;
                case 0x18:
                    // Set sound timer = VX
                    // SSR (Set Sound timer to Register)
                    printf("SSR\tV[%x]", code[0]&0xf);
                    break;
                case 0x1e:
                    // I += VX
                    // AIR (Add I and Register)
                    printf("AIR\tV[%x]", code[0]&0xf);
                    break;
                case 0x29:
                    // I = location of sprite for digit VX
                    // SETH (SET Hex)
                    printf("SETH\tV[%x]", code[0]&0xf);
                    break;
                case 0x33:
                    // Store BCD representation of VX in memory location I, I+1, I+2
                    // BCD
                    printf("BCD\tV[%x]", code[0]&0xf);
                    break;
                case 0x55:
                    // Push all registers V[0..X] to location I
                    // PUSHA
                    printf("PUSHA\tV[%x]", code[0]&0xf);
                    break;
                case 0x65:
                    // Reads from memory to registers V[0..X] starting from I
                    // POPA
                    printf("POPA\tV[%x]", code[0]&0xf);
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
    fread(buffer, file_size, 1, f);

    fclose(f);



    unsigned short offset = 0;
    if (padded)
        offset = 0x200;



    for (unsigned int pc=0; pc<file_size; pc+=2) {
        emulateStep(buffer, pc, clean, offset);
    }

    return 0;
}
