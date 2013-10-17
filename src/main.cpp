#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <cmath>

#include <SDL2/SDL.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "input/AllegroKeyboard.h"
#include "input/AllegroJoystick.h"

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

#include <Voicer.h>
#include <WvOut.h>
#include <JCRev.h>

#include "VolumeSine.h"
#include "RtAudio.h"

using namespace stk;
using namespace std;

// The TickData structure holds all the class instances and data that
// are shared by the various processing functions.
struct TickData {
    Instrmnt **instrument;
    Voicer *voicer;
    JCRev reverb;
    StkFloat volume;
    StkFloat t60;
    int nVoices;
    int channels;
    int counter;
    int frequency;
    
    // Default constructor.
    TickData() : instrument(0), voicer(0), volume(1.0), t60(0.75), nVoices(1), channels(2), counter(0) {}
};





StkFrames frames(RT_BUFFER_SIZE, 2);


int currentInstr = 0;

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
    TickData* data = (TickData*) dataPointer;
    register StkFloat sample;
    register StkFloat *samples = (StkFloat *) outputBuffer;
    
    for ( unsigned int i=0; i<nBufferFrames; i++ ) {
        sample = data->volume * data->reverb.tick( data->voicer->tick() );
        for ( int k=0; k<data->channels; k++ )
            *samples++ = sample;
    }
    
    return 0;
}

volatile bool running = true;
StkFloat halfNote = 1.0 / 12.0;
StkFloat invPitch = 1.0 / 440.0;
StkFloat invln2 = 1.0 / log(2.0);
StkFloat e = exp(1.0);
int minNote = 25;
int maxNote = 54;


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

int group = 0;
bool triggered[] = { false, false, false, false };
long noteIds[] = { -1, -1, -1, -1, -1, -1 };
int notes[] = { -1, -1, -1, -1, -1, -1 };
TickData data;
StkFloat amplitude;
double noteOff = 0;
int note = -1;

void onJoystickButtonDown(int button) {
    if (button == 8) {
        group--;
    } else if (button == 9) {
        group++;
    } else if (button == 1 || button == 0) {
        if (triggered[group]) {
            noteIds[group] = data.voicer->noteOn(note, amplitude*128.0, group);
            notes[group] = note;
        } else if (note == notes[group]) {
            data.voicer->noteOff(noteIds[group], 0.0);
            noteOff = al_get_time();
        }
    } else if (button >= 2) {
        int g = button+1;
        if (notes[g] < 0) {
            noteIds[g] = data.voicer->noteOn(39, 128.0, g);
            notes[g] = 1;
        } else {
            data.voicer->noteOff(noteIds[g], 128.0);
            notes[g] = -1;
        }
    }
    
    if (group < 0) {
        group = 0;
    }
    if (group > 3) {
        group = 3;
    }
}

int main(int argc, char** argv)
{
    float position = 0.0f;
    
	al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    if (!al_install_joystick())
        std::cerr << "ERROR: could not initialize input device driver" << std::endl;
    if (al_get_num_joysticks() <= 0)
        std::cerr << "ERROR: no input device found" << std::endl;
    
    AllegroKeyboard keyboard;
    AllegroJoystick joystick;
    
    al_set_new_display_flags(ALLEGRO_OPENGL);
    ALLEGRO_DISPLAY* display = al_create_display(640, 480);
    
    
    
    if(!display){
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }
    
    ALLEGRO_FONT* font = al_load_font("OpenSans-Regular.ttf", 20, 0);
    
    // Set the global sample rate before creating class instances.
    Stk::setSampleRate( 44100.0 );
    Stk::setRawwavePath("./rawwaves");
    
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
    
   // test.addFileLoop(new FileLoop("samples/ONE_SHOTS_128bpm/One_Shot_01_D#.wav"));
    
    int voiceCount = 8;
    
    Instrmnt** instr = new Instrmnt*[voiceCount];
    instr[0] = &ov;
    instr[1] = &ov2;
    instr[2] = &voices;
    instr[3] = &rhodey;
    instr[4] = &sample;
    instr[5] = &sample2;
    instr[6] = &sample3;
    instr[7] = &sample4;
    
    RtAudio dac;

    data.nVoices = voiceCount;
    data.instrument = instr;
    
    data.voicer = new Voicer(0.0);
    for ( int i=0; i<data.nVoices; i++ )
        data.voicer->addInstrument( data.instrument[i], i );
    
    
    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 2;
    RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    unsigned int bufferFrames = RT_BUFFER_SIZE;
    try {
        dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&data );
    }
    catch ( RtError &error ) {
        error.printMessage();
    }
    
    data.reverb.setT60( data.t60 );
    data.reverb.setEffectMix(0.2);
    
    data.volume = 1.0;
    
    
    try {
        dac.startStream();
    }
    catch ( RtError &error ) {
        error.printMessage();
    }

    StkFloat frequency = 440.0;
    
    
    double offset;
    
    while (running) {
        joystick.update();
        keyboard.update();
        
        if (keyboard.isButtonDown(ALLEGRO_KEY_ESCAPE))
            running = false;
        if (keyboard.isButtonDown(ALLEGRO_KEY_ENTER))
            offset = -joystick.getAxis(0);
        
        
        
        char text[255];
        char text2[255];
        stringstream text3;
        char text4[255];
        
        
        
        StkFloat rawfrequency = 440.0 + (joystick.getAxis(0)+offset)*500.0;
        amplitude = log((1.0-joystick.getAxis(1)) * (e-1)*0.5 + 1.0);
        
        StkFloat rawnote = (joystick.getAxis(0) + 1.0)*0.5*(maxNote - minNote) + minNote;
        StkFloat middle = 0.5*(maxNote - minNote) + minNote;
        
        for (int i = 0; i < 32; ++i) {
            text3 << joystick.isButtonDown(i) << " ";
        }
        
        if (joystick.isButtonDown(0)) {
            note = getNote(rawnote) + 1;
        }
        else if (joystick.isButtonDown(1)) {
            note = getNote(rawnote);
        }
        
            //frequency = getFrequency(note);
        //instr[currentInstr]->noteOn(frequency, amplitude*1.0);
        if (noteIds[group] < 0 && note > 0) {
            
        } else {
            //data.voicer->noteOff(notes[group], 0.0);
            //data.voicer->setFrequency(notes[group], (StkFloat)note);
        }
        if (!triggered[group]) {
            if (al_get_time() - noteOff > 0.05 && note > 0) {
                noteIds[group] = data.voicer->noteOn(note, amplitude*128.0, group);
                notes[group] = note;
            }
        }
        
        
        
        //al_clear_to_color(al_map_rgb(0,0,0));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        setupOpenGlMatrices(float((rawnote-middle)*0.1f), float(position));
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

        sprintf(text, "%f, %f, %f, %f", offset, joystick.getAxis(0), joystick.getAxis(1), joystick.getAxis(2));
        sprintf(text2, "%f -> %f, %f, %f, %s, %d", rawfrequency, frequency, amplitude, rawnote, getNoteName(getNote(rawnote)).c_str(), currentInstr);
        sprintf(text4, "Group: %d", group);
        al_draw_text(font, al_map_rgb(255, 255, 255), 50, 50, 0, text);
        al_draw_text(font, al_map_rgb(255, 255, 255), 50, 100, 0, text2);
        al_draw_text(font, al_map_rgb(255, 255, 255), 50, 150, 0, text3.str().c_str());
        al_draw_text(font, al_map_rgb(255, 255, 255), 50, 200, 0, text4);
        
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
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
	return 0;
}