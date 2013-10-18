#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <cmath>

#include <SDL2/SDL.h>

#include "engine/allegro/AllegroSystem.h"
#include "engine/Keyboard.h"
#include "engine/Joystick.h"
#include "sound/stk/StkSynthesizer.h"
#include <Wheeloin.h>
#include <Score.h>
#include <Scale.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <Saxofony.h>
#include <HevyMetl.h>
#include <BeeThree.h>
#include <Bowed.h>
#include <Simple.h>
#include <BandedWG.h>
#include <Brass.h>
#include <Moog.h>
#include <Shakers.h>
#include <VoicForm.h>
#include <Whistle.h>
#include <FMVoices.h>
#include <PercFlut.h>
#include <TubeBell.h>
#include <Rhodey.h>
#include <Wurley.h>
#include "SampleInstrument.h"
#include "Overdrive.h"

#include "VolumeSine.h"

using namespace stk;
using namespace std;

volatile bool running = true;
StkFloat halfNote = 1.0 / 12.0;
StkFloat invPitch = 1.0 / 440.0;
StkFloat invln2 = 1.0 / log(2.0);
StkFloat e = exp(1.0);


void initOpenGl() {
    glEnable (GL_LINE_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth (1.5f);
}



int main(int argc, char** argv)
{
    
    
    AllegroSystem system(1440, 900);
    StkSynthesizer synth("./rawwaves");
    
    system.getRenderer().setTextFont("OpenSans-Regular.ttf", 20);
    system.getRenderer().setTextColor(255, 255, 255);
    
    
    HevyMetl heavyMetl;
    Whistle whistle;
    VoicForm voices;
    // Noise:
    whistle.controlChange(4, 10.0);
    VoicForm sing;
    FMVoices fmVoices;
    Rhodey rhodey;
    TubeBell bell;
    
    
    SampleInstrument sample("../samples/PERCUSSION_128bpm/Percussion_03_Full.wav");
    SampleInstrument sample2("../samples/BASSLINES_128bpm/Bass_01_C.wav");
    SampleInstrument sample3("../samples/BASSLINES_128bpm/Bass_02_D#.wav");
    SampleInstrument sample4("../samples/BASSLINES_128bpm/Bass_03_E.wav");
    
    Overdrive ov(&heavyMetl);
    ov.setOverdrive(5.0);
    
    Overdrive ov2(&whistle);

    synth.addInstrument(&ov);
    synth.addInstrument(&sing);
    synth.addInstrument(&fmVoices);
    
    WheeloinConfiguration config(Scales::MAJOR, 25, 54);
    Wheeloin wheeloin(synth, system, config);
    
    Score score(wheeloin, system.getRenderer());
    
    synth.start();
    
    while (running) {
        system.updateInput();
        
        if (system.getKeyboard().isButtonDown(ALLEGRO_KEY_ESCAPE))
            running = false;
       
        wheeloin.processInput();
        
       
        
        
        score.draw();
        system.getRenderer().updateDisplay();
    }
    synth.stop();
	return 0;
}