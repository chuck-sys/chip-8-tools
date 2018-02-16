### Options

```
./Chip8_Emulator [options] <chip8_bin>

Options:
  -h, --help               Shows this help text

  -bg <color>,
  --background <color>     Set custom background color (32-bit RGBA)

  -fg <color>,
  --foreground <color>     Set custom foreground color (32-bit RGBA)

  -f <fps>,
  --fps <fps>              Set frames per second
```

```sh
./Chip8_Emulator PONG
./Chip8_Emulator -bg 3722304989 PONG    # Decimal for 0xDDDDDDDD
./Chip8_Emulator -f 5 PONG
```

### Usage

#### The Chip-8 keypad

The Chip-8 hex keypad is mapped to the QWERTY keyboard from this:

```
1 2 3 C
4 5 6 D
7 8 9 E
A 0 B F
```

To this:

```
1 2 3 4
Q W E R
A S D F
Z X C V
```
