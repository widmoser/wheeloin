#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <cmath>

#include <SDL2/SDL.h>

#include "engine/allegro/AllegroSystem.h"
#include "engine/Keyboard.h"
#include "engine/Joystick.h"
#include "WheeloinSynth.h"
#include <Wheeloin.h>
#include <ScoreDisplay.h>
#include <TrackScoreDisplay.h>
#include <Scale.h>
#include <Exception.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "VolumeSine.h"

using namespace stk;
using namespace std;

int main(int argc, char** argv)
{
    try {
        volatile bool running = true;
        
        AllegroSystem system(1440, 900);
        system.getRenderer().setTextFont("OpenSans-Regular.ttf", 20);
        system.getRenderer().setTextColor(255, 255, 255);
        
        Score score("test.score");
        WheeloinSynth synth;
        WheeloinConfiguration config(Scales::MAJOR, 25, 54);
        Wheeloin wheeloin(synth, system, config, score);
        
        ScoreDisplay scoreDisplay(wheeloin, system, score);
        
        synth.start();
        while (running) {
            system.updateInput();
            if (system.getKeyboard().isButtonDown(ALLEGRO_KEY_ESCAPE))
                running = false;
            wheeloin.processInput();
            scoreDisplay.draw();
            system.getRenderer().updateDisplay();
        }
        synth.stop();
        
        return 0;
    } catch (Exception e) {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    }
}