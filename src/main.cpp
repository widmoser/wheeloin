#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <cmath>

#include <SDL2/SDL.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "engine/allegro/AllegroSystem.h"
#include "engine/Keyboard.h"
#include "engine/Joystick.h"
#include "sound/stk/StkSynthesizer.h"
#include <Wheeloin.h>
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


string names[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "H"
};

int majorScale[] = {
    0, 2, 4, 5, 7, 9, 11
};

StkFloat getFrequency(StkFloat note) {
    return 440 * pow(2, (note - 69) * halfNote);
}

StkFloat getRawNote(StkFloat frequency) {
    return 12*log(frequency*invPitch)*invln2 + 69;
}

string getNoteName(int note) {
    if (note > 4) {
        return names[(note-4) % 12];
    } else {
        return "";
    }
}

int getNote(StkFloat rawnote) {
    // rawnote is in terms of scale:
    int rounded = int(round(rawnote));
    int octave = rounded / 7;
    int note = rounded % 7;
    std::cout << rounded << " " << octave << " " << note << std::endl;
    return octave*12 + majorScale[note];
}

void initOpenGl() {
    glEnable (GL_LINE_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth (1.5f);
}

void setupOpenGlMatrices(float note, float time) {
    glMatrixMode(GL_PROJECTION); //Open the projection matrix as current
    glLoadIdentity(); //clear its contents to an identity matrix (which doesn't change anything when applied)
    gluPerspective(60, 1.0, 0.1, 1000.0);
    //this function is from the very useful glu library .
    //It configures the projection matrix to the desired parameters
    
    glMatrixMode(GL_MODELVIEW); //select the matrix
    glLoadIdentity(); //clear ...
    gluLookAt ( note , 1, -time, note, 0, -time-1, 0, 1, 0 );
}

int main(int argc, char** argv)
{
    float position = 0.0f;
    
    AllegroSystem system;
    
    al_init_font_addon();
    al_init_ttf_addon();
    
    al_set_new_display_flags(ALLEGRO_OPENGL);
    ALLEGRO_DISPLAY* display = al_create_display(640, 480);
    
    StkSynthesizer synth("./rawwaves");

    if(!display){
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }
    
    ALLEGRO_FONT* font = al_load_font("OpenSans-Regular.ttf", 20, 0);
    
    
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
    
    synth.start();

    StkFloat frequency = 440.0;
    
    
    double offset;
    
    while (running) {
        system.updateInput();
        
        if (system.getKeyboard().isButtonDown(ALLEGRO_KEY_ESCAPE))
            running = false;
       
        wheeloin.processInput();
        
        
        char text[255];
        char text2[255];
        stringstream text3;
        char text4[255];
        
        double middle = 0.5*(config.maxNote - config.minNote) + config.minNote;

        
        
        for (int i = 0; i < 32; ++i) {
            text3 << system.getJoystick().isButtonDown(i) << " ";
        }
        
        //al_clear_to_color(al_map_rgb(0,0,0));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        setupOpenGlMatrices(float((wheeloin.getInputScaleNote()-middle)*0.1f), float(position));
        position += 0.01f;
        glBegin(GL_LINES); //lookup for more options but GL_LINES tels GL that you'll be supplying a series of lines (via points)
        
        glColor3f(0.2f, 0.2f, 0.2f);
        
        for (int i = -50; i < 50; ++i) {
            glVertex3f(i*0.1f-0.05f,0.0f,100.0f);
            glVertex3f(i*0.1f-0.05f,0.0f,-position-100.0f);
        }
        
        for (int i = -500; i < 500; ++i) {
            glVertex3f(-100,0,i);
            glVertex3f(100,0,i);
        }
        
        glEnd(); //this will draw a single line watched from the camera that we just set up
        
        glBegin(GL_QUADS);
        
        glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
        
        glVertex3f(-0.05f, 0.0f, -50.0f);
        glVertex3f(0.05f, 0.0f, -50.0f);
        glVertex3f(0.05f, 0.0f, -100.0f);
        glVertex3f(-0.05f, 0.0f, -100.0f);
        
        glEnd();
        
        
        glFlush();
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 640, 480, 0.0, -1.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_CULL_FACE);

        sprintf(text, "%f, %f, %f, %f", offset, system.getJoystick().getAxis(0), system.getJoystick().getAxis(1), system.getJoystick().getAxis(2));
        sprintf(text2, "%f, %f, %s", wheeloin.getVolume(), wheeloin.getInputScaleNote(), getNoteName(getNote(wheeloin.getInputScaleNote())).c_str());
        sprintf(text4, "Group: %d", synth.getActiveVoice());
        al_draw_text(font, al_map_rgb(255, 255, 255), 50, 50, 0, text);
        al_draw_text(font, al_map_rgb(255, 255, 255), 50, 100, 0, text2);
        al_draw_text(font, al_map_rgb(255, 255, 255), 50, 150, 0, text3.str().c_str());
        al_draw_text(font, al_map_rgb(255, 255, 255), 50, 200, 0, text4);
        
        //instr.setFrequency(frequency);
        //instr.controlChange(128, amplitude*100.0);
        
        al_flip_display();
    }
    synth.stop();
    
    al_destroy_font(font);
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
	return 0;
}