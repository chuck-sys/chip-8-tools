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
    for (int i=0; i<Samples; i++) {
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
                c8cpu->handleKeyPress(evt.key.code);
            }
            else if (evt.type == Event::KeyReleased) {
                // Handle key released
                c8cpu->handleKeyUp(evt.key.code);
            }
        }

        // Run one step of the code
        c8cpu->emulateStep();

        // Error handling (exit on error)
        if (c8cpu->error) {
            cerr << "Error: " << c8cpu->error_txt << endl;
            window->close();
        }
        // Output status text if there is any
        if (!c8cpu->status_txt.empty()) {
            cerr << c8cpu->status_txt << endl;
            c8cpu->status_txt = "";
        }

        // Drawing
        if (c8cpu->drawf) {
            window->clear(bg);
            c8cpu->drawDisplay(window, bg, fg);
            window->display();
        }

        // Sound stuff
        if (hasSound) {
            if (c8cpu->st > 0) {
                // Only activate if sound is greater than 0
                if (beep.getStatus() == Sound::Status::Paused ||
                        beep.getStatus() == Sound::Status::Stopped) {
                    // Doesn't play if the sound is already playing
                    beep.play();
                }
            } else {
                beep.pause();
            }
        }

        // Query FPS
        window->setTitle(title + " | FPS: " + to_string(1/c.getElapsedTime().asSeconds()));

        // Clock FPS
        while (c.getElapsedTime().asSeconds() < 1./60) {}
        c.restart();
    }


    return 0;
}
