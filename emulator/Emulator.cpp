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

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <memory>

#include <config.h>

#include "Chip_8.h"

using namespace std;
using namespace sf;

const unsigned Samples = 44100;
const unsigned Sample_Rate = 44100;
const unsigned Amptitude = 30000;
const double TAU = 6.28318;
const double Increment = 440. / 44100;

Color bg = Color::Black, fg = Color::White;

int main(int argc, char **argv) {
    // Arguments checking
    if (argc == 1) {
        cerr << "Error: Invalid number of arguments.\n"
            << "Usage: " << argv[0] << " <program_file>\n";
        return -1;
    }

    // Arguments parsing
    int ind = 1;
    unsigned fps = 60;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            cout << "Chip 8 Emulator " VERSION " by Cheuk Yin Ng\n"
                "Report all bugs to <" REP_ADDR ">.\n\n"
                "Usage:\n"
                << argv[0] << " [options] <chip8_bin>\n\n"
                "Options:\n"
                "  -h, --help               Shows this help text\n\n"
                "  -bg <color>,\n"
                "  --background <color>     Set custom background color (32-bit RGBA)\n\n"
                "  -fg <color>,\n"
                "  --foreground <color>     Set custom foreground color (32-bit RGBA)\n\n"
                "  -f <fps>,\n"
                "  --fps <fps>              Set frames per second\n";
            return 0;
        }
        else if (strcmp(argv[i], "-bg") == 0 || strcmp(argv[i], "--background") == 0) {
            if (++i < argc) {
                bg = Color(atoi(argv[i]));
            }
            else {
                cerr << "Expected an integer.\n";
                return -1;
            }
        }
        else if (strcmp(argv[i], "-fg") == 0 || strcmp(argv[i], "--foreground") == 0) {
            if (++i < argc) {
                fg = Color(atoi(argv[i]));
            }
            else {
                cerr << "Expected an integer.\n";
                return -1;
            }
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fps") == 0) {
            if (++i < argc) {
                fps = atoi(argv[i]);
                if (fps == 0) {
                    cerr << "That isn't a valid integer.\n";
                    return -1;
                }
            }
            else {
                cerr << "Expected an integer.\n";
                return -1;
            }
        }
        else {
            // Or else it will be that argument
            ind = i;
        }
    }

    unique_ptr<Chip_8> c8cpu(new Chip_8());
    shared_ptr<RenderWindow> window(new RenderWindow(VideoMode(800, 500),
                                                     argv[ind],
                                                     Style::Close));
    Clock c;

    // Initializations with the chip8 cpu
    c8cpu->initialize(argv[ind]);

    // Sound initializations
    Int16 raw[Samples];
    double x = 0;
    for (unsigned i = 0; i < Samples; i++) {
        raw[i] = Amptitude * sin(x * TAU);
        x += Increment;
    }
    SoundBuffer buffer;
    bool hasSound = true;
    if (!buffer.loadFromSamples(raw, Samples, 1, Sample_Rate)) {
        cerr << "Error: Sound buffer failed to load!\n";
        hasSound = false;
    }
    Sound beep;
    beep.setBuffer(buffer);
    beep.setLoop(true);

    // Main loop
    while (window->isOpen()) {
        Event evt;
        while (window->pollEvent(evt)) {
            if (evt.type == Event::Closed)
                window->close();
            else if (evt.type == Event::KeyPressed) {
                // Handle key pressed
                c8cpu->handleKey(evt.key.code, true);
            }
            else if (evt.type == Event::KeyReleased) {
                // Handle key released
                c8cpu->handleKey(evt.key.code, false);
            }
        }

        // Run one step of the code
        c8cpu->emulateStep();

        // Error handling (exit on error)
        if (c8cpu->error) {
            cerr << "Error: " << c8cpu->error_txt << endl;
            window->close();
        }

        // Drawing
        if (c8cpu->drawf) {
            window->clear(bg);
            c8cpu->drawDisplay(window.get(), bg, fg);
            window->display();

            // Stay at specified framerate
            float sleep_time = 1. / fps-c.getElapsedTime().asSeconds();
            if (sleep_time > 0) {
                sleep(seconds(sleep_time));
            }
            c.restart();
        }

        // Sound stuff
        if (hasSound) {
            if (c8cpu->st > 0) {
                // Only activate if sound is greater than 0
                if (beep.getStatus() != Sound::Status::Playing) {
                    // Doesn't play if the sound is already playing
                    beep.play();
                }
            } else {
                beep.pause();
            }
        }
    }

    return 0;
}
