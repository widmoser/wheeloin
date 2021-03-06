//
//  Wheeloin.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Wheeloin.h"

const double Wheeloin::e = std::exp(1.0);

Wheeloin::Wheeloin(Synthesizer& synth, System& system, WheeloinConfiguration configuration) : synth(synth), system(system), note(-1), amplitude(0.0), noteOff(0.0), inputScaleNote(-1.0), inputNote(-1), offset(0.0), keyboard(system.getKeyboard()), joystick(system.getJoystick()), conf(configuration), continuous(false) {
    triggered.resize(synth.getVoiceCount(), false);
    joystick.registerButtonListener(this);
}

void Wheeloin::setTriggered(int voice, bool value) {
    triggered[voice] = value;
}

void Wheeloin::onButtonDown(int button) {
    
    
    if (button == 8) {
        synth.setActiveVoice(synth.getActiveVoice()-1);
        note = -1;
    } else if (button == 9) {
        synth.setActiveVoice(synth.getActiveVoice()+1);
        note = -1;
    } else if (button == 1 || button == 0) {
        if (triggered[synth.getActiveVoice()]) {
            synth.noteOn(note, amplitude*64.0);
        } else if (note == synth.getNote()) {
            synth.noteOff();
            noteOff = system.getTime();
        }
    } else if (button >= 2) {
        int g = button+2;
        if (!synth.isNoteOn(g)) {
            synth.noteOn(39, 64.0, g);
        } else {
            synth.noteOff(g);
        }
    }
}

void Wheeloin::onButtonUp(int button) {
// AUTOMATIC:
//    if (button == 1 || button == 0) {
//        if (score.nextNote().voice != getActiveVoice()) {
//            synth.setActiveVoice(score.nextNote().voice);
//            note = -1;
//        }
//    }
}

void Wheeloin::processInput() {
    amplitude = log((1.0-joystick.getAxis(3)) * (e-1)*0.5 + 1.0);
    
    if (amplitude < 0.01) {
        amplitude = 0.0;
    }
    
    inputScaleNote = (joystick.getAxis(0) + 1.0)*0.5*(conf.maxNote - conf.minNote) + conf.minNote;
    if (continuous) {
        inputNote = inputScaleNote;
        note = inputScaleNote;
    } else {
        inputNote = conf.scale.getNote(inputScaleNote);
        if (joystick.isButtonDown(0)) {
            note = inputNote + 1;
        }
        else if (joystick.isButtonDown(1)) {
            note = inputNote;
        }
    }
    
    if (!triggered[synth.getActiveVoice()]) {
        if (system.getTime() - noteOff > 0.05 && note > 0) {
            synth.noteOn(note, amplitude*64.0);
        }
    }
    
    if (keyboard.isButtonDown(67 /*ENTER*/))
        offset = -joystick.getAxis(0);
    
    if (amplitude < 0.01) {
        note = -1;
    }
}