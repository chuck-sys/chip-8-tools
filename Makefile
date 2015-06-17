CC := g++
CFLAGS := -g -c -std=c++0x
LD := g++
SFMLLDFLAGS := -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
LDFLAGS :=

EMULATOR := Chip-8_Emulator
DISASSEMBLER := Chip-8_Disassembler
SRC := src/Chip_8.cpp src/Emulator.cpp src/Disassembler.cpp
EMU_OBJ := obj/Chip_8.o obj/Emulator.o
DISA_OBJ := obj/Disassembler.o

all: $(EMULATOR) $(DISASSEMBLER)

emulator: $(EMULATOR)

disassembler: $(DISASSEMBLER)

$(DISASSEMBLER): obj/ $(DISA_OBJ)
	$(LD) -o $@ $(LDFLAGS) $(DISA_OBJ)

$(EMULATOR): obj/ $(EMU_OBJ)
	$(LD) -o $@ $(SFMLLDFLAGS) $(EMU_OBJ)

obj/:
	mkdir $@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXE)
	rm -f $(EMU_OBJ)
	rm -f $(DISA_OBJ)
