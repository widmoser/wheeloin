//
//  Synthesizer.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Synthesizer.h"

Synthesizer::Synthesizer() : activeVoice(0) {}

int Synthesizer::getActiveVoice() {
    return activeVoice;
}

void Synthesizer::setActiveVoice(int voice) {
    if (voice < 0) {
        activeVoice = 0;
    } else if (voice >= getVoiceCount()) {
        activeVoice = getVoiceCount() - 1;
    } else {
        activeVoice = voice;
    }
}

void Synthesizer::noteOn(double note, double amplitude) {
    noteOn(note, amplitude, activeVoice);
}

bool Synthesizer::isNoteOn() {
    return isNoteOn(activeVoice);
}

double Synthesizer::getNote() {
    return getNote(activeVoice);
}

void Synthesizer::noteOff() {
    noteOff(activeVoice);
}