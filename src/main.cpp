#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <cmath>

#include "engine/sfml/SFMLSystem.h"
#include "engine/Keyboard.h"
#include "engine/Joystick.h"
#include <TrackScoreDisplay.h>
#include <Exception.h>
#include <Piece.h>
#include <RoundComputation.h>
#include <Pause.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "VolumeSine.h"

using namespace stk;
using namespace std;

int argContains(int argc, char** argv, const char* option) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], option) == 0) {
            return i;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    try {
        volatile bool running = true;
        
        SFMLSystem system(1440, 900);
        system.getRenderer().setTextColor(255, 255, 255);

        
        std::cout << "1" << std::endl;
        WheeloinSynth synth;
        std::cout << "2" << std::endl;
        Wheeloin instrument(synth, system, WheeloinConfiguration(Scales::MAJOR, 15, 54));
        
        Score score1;
        Piece piece1("I. XII in a round", system, instrument, score1);
        
        Score score2;
        Piece piece2("II. Fragments", system, instrument, score2);
        
        double rythm[12] = {
          4, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2
        };
        Parameters params(3, 4.0, rythm);
        RoundComputation computation(system, params, 8, piece1);
        
        Pause pause(system);
        
        std::queue<Phase*> phases;
        int argInput = argContains(argc, argv, "-i");
        if (!argInput) {
            phases.push(&computation);
        } else {
            Series s = RoundComputation::fillScore(score1);
            //RoundComputation::computeFragmentedScore(score2, params, s.data);
        }
        
        phases.push(&pause);
        phases.push(&piece2);
        phases.push(&piece1);

        phases.front()->init();
        synth.start();
        while (running) {
            system.updateInput();
            if (system.getKeyboard().isButtonDown(sf::Keyboard::Escape)) {
                running = false;
                phases.front()->onCancel();
            } else {
                if (!phases.front()->frame()) {
                    if (phases.size() > 1) {
                        // next phase
                        phases.front()->finalize();
                        phases.pop();
                        phases.front()->init();
                    } else {
                        running = false;
                    }
                }
                system.getRenderer().updateDisplay();
            }
        }
        synth.stop();
        
        return 0;
    } catch (Exception e) {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    }
}