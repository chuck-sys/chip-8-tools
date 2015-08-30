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
#include <iostream>
#include <string>

#include "Chip_8.h"

using namespace std;
using namespace sf;

const unsigned Samples = 44100;
const unsigned Sample_Rate = 44100;
const unsigned Amptitude = 30000;
const double TAU = 6.28318;
const double Increment = 440./44100;

Color bg, fg;

int main(int argc, char **argv) {
    // Arguments checking
    if (argc == 1) {
        cerr << "Error: Invalid number of arguments.\n"
            << "Usage: " << argv[0] << " <program_file>\n";
        return -1;
    }

    Chip_8 *c8cpu = new Chip_8();
    bg = Color::Black;
    fg = Color::White;
    string title = argv[1];
    RenderWindow *window = new RenderWindow(VideoMode(800, 500), argv[1]);      // Filename is the title
    Clock c;

    // Initializations with the chip8 cpu
    c8cpu->initialize(argv[1]);

    // Sound initializations
    Int16 raw[Samples];
    double x = 0;
    for (unsigned i=0; i<Samples; i++) {
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
            c8cpu->drawDisplay(window, bg, fg);
            window->display();

            // Stay at 60 FPS
            float sleep_time = 1/60.-c.getElapsedTime().asSeconds();
            if (sleep_time > 0)
                sleep(seconds(sleep_time));
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
