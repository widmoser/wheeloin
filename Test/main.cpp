#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <cmath>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <Saxofony.h>
#include <HevyMetl.h>
#include <BeeThree.h>
#include <Bowed.h>
#include <Voicer.h>

#include "VolumeSine.h"
#include "RtAudio.h"
using namespace stk;
using namespace std;

StkFrames frames(RT_BUFFER_SIZE, 2);


int currentInstr = 0;

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
    Instrmnt *instr = ((Instrmnt **) dataPointer)[currentInstr];
    register StkFloat *samples = (StkFloat *) outputBuffer;
    
    for ( unsigned int i=0; i<nBufferFrames; i++ ) {
        *samples++ = instr->tick(0);
        *samples++ = instr->tick(1);
    }
    
    return 0;
}

volatile bool running = true;
StkFloat halfNote = 1.0 / 12.0;
StkFloat invPitch = 1.0 / 440.0;
StkFloat invln2 = 1.0 / log(2.0);
StkFloat e = exp(1.0);
int minNote = 15;
int maxNote = 69;


string names[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "H"
};

StkFloat getFrequency(StkFloat note) {
    return 440 * pow(2, (note - 69) * halfNote);
}

StkFloat getNote(StkFloat frequency) {
    return 12*log(frequency*invPitch)*invln2 + 69;
}

string getNoteName(int note) {
    if (note > 4) {
        return names[(note-4) % 12];
    } else {
        return "";
    }
}



int main(int argc, char** argv)
{
	al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    if (!al_install_joystick())
        std::cerr << "ERROR: could not initialize input device driver" << std::endl;
    if (al_get_num_joysticks() <= 0)
        std::cerr << "ERROR: no input device found" << std::endl;
    
    ALLEGRO_KEYBOARD_STATE keyboardstate;
    
    al_install_keyboard();
    
    ALLEGRO_JOYSTICK* input_device = al_get_joystick(0);
    ALLEGRO_JOYSTICK_STATE state;
    
    al_create_display(640, 480);
    
    ALLEGRO_FONT* font = al_load_font("OpenSans-Regular.ttf", 20, 0);
    
    // Set the global sample rate before creating class instances.
    Stk::setSampleRate( 44100.0 );
    Stk::setRawwavePath("./rawwaves");
    
    HevyMetl instr0;
    BeeThree instr1;
    Bowed instr2;
    
    Voicer voicer;
    voicer.addInstrument(&instr1);
    
    voicer.noteOn(64, 0.3);
    voicer.noteOn(67, 0.3);
    
    Instrmnt** instr = new Instrmnt*[4];
    instr[0] = &instr0;
    instr[1] = &instr1;
    instr[2] = &instr2;
    
    RtAudio dac;
    
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 2;
    RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    unsigned int bufferFrames = RT_BUFFER_SIZE;
    try {
        dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)instr );
    }
    catch ( RtError &error ) {
        error.printMessage();
    }
    
    instr[currentInstr]->noteOn(440.0, 0.0);
    
    try {
        dac.startStream();
    }
    catch ( RtError &error ) {
        error.printMessage();
    }
        
    bool noteOn = false;
    StkFloat frequency = 440.0;
    
    ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_joystick_event_source());
    
    while (running) {
        al_get_joystick_state(input_device, &state);
        al_get_keyboard_state(&keyboardstate);
        
        if (al_key_down(&keyboardstate, ALLEGRO_KEY_ESCAPE))
            running = false;
        
        al_clear_to_color(al_map_rgb(255,255,255));
        
        char text[255];
        char text2[255];
        stringstream text3;
        
        ALLEGRO_EVENT event;
        if (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
                if (event.joystick.button == 8) {
                    currentInstr--;
                } else if (event.joystick.button == 9) {
                    currentInstr++;
                }
                if (currentInstr < 0) {
                    currentInstr = 0;
                }
                if (currentInstr > 2) {
                    currentInstr = 2;
                }
            }
        }
        
        StkFloat rawfrequency = 440.0 + state.stick->axis[0]*500.0;
        StkFloat amplitude = log((1.0-state.stick->axis[1]) * (e-1)*0.5 + 1.0);
        
        amplitude *= (1.0 - state.stick->axis[2])*2.0 + 1.0;
        
        StkFloat rawnote = (state.stick->axis[0] + 1.0)*0.5*(maxNote - minNote) + minNote;
        int note = round(rawnote);
        //StkFloat frequency = getFrequency(round(note));
        
        
        sprintf(text, "%f, %f, %f", state.stick->axis[0], state.stick->axis[1], state.stick->axis[2]);
        sprintf(text2, "%f -> %f, %f, %f, %s, %d", rawfrequency, frequency, amplitude, rawnote, getNoteName(note).c_str(), currentInstr);
        
        for (int i = 0; i < 32; ++i) {
            text3 << state.button[i] << " ";
        }
        
        //if (-state.stick->axis[2] > 0)
        if (state.button[1])
            frequency = getFrequency(note);
        instr[currentInstr]->noteOn(frequency, amplitude*1.0);

        al_draw_text(font, al_map_rgb(0, 0, 0), 50, 50, 0, text);
        al_draw_text(font, al_map_rgb(0, 0, 0), 50, 100, 0, text2);
        al_draw_text(font, al_map_rgb(0, 0, 0), 50, 150, 0, text3.str().c_str());
        
        //instr.setFrequency(frequency);
        //instr.controlChange(128, amplitude*100.0);
        
        al_flip_display();
    }
    
    try {
        dac.closeStream();
    }
    catch ( RtError &error ) {
        error.printMessage();
    }
    
    al_destroy_font(font);
    al_destroy_event_queue(event_queue);
    al_release_joystick(input_device);
    al_uninstall_joystick();
    al_uninstall_keyboard();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
	return 0;
}