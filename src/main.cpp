#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <cmath>

#include <SDL2/SDL.h>

#include "engine/allegro/AllegroSystem.h"
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

//#include "MusicNoteLib.h"

using namespace stk;
using namespace std;

int main(int argc, char** argv)
{
    try {
        volatile bool running = true;
        
        //MusicNoteLib::PlayMusicString("C D E F G A B"); // Play the Music Notes on the default MIDI output port
        
        AllegroSystem system(1440, 900);
        system.getRenderer().setTextFont("OpenSans-Regular.ttf", 20);
        system.getRenderer().setTextColor(255, 255, 255);
        
        Score score("test.score");
        Piece piece(system, score);
        
        double rythm[12] = {
          2, 1, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 1
        };
        Parameters params(3, 4.0, rythm);
        RoundComputation computation(system, params, 8);
        
        Pause pause(system);
        
        std::queue<Phase*> phases;
        phases.push(&computation);
        phases.push(&pause);
        phases.push(&piece);
        
        phases.front()->init();
        while (running) {
            system.updateInput();
            if (system.getKeyboard().isButtonDown(ALLEGRO_KEY_ESCAPE)) {
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
        
        return 0;
    } catch (Exception e) {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    }
}