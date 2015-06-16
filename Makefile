CC := g++
CFLAGS := -g -c -std=c++0x
LD := g++
LDFLAGS := -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

EXE := Chip-8_Emulator
SRC := src/Chip_8.cpp src/Emulator.cpp
OBJ := obj/Chip_8.o obj/Emulator.o

all: $(EXE)

$(EXE): obj/ $(OBJ)
	$(LD) -o $@ $(LDFLAGS) $(OBJ)

obj/:
	mkdir $@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXE)
	rm -f $(OBJ)
