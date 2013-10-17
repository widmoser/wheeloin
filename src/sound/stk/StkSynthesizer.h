//
//  StkSynthesizer.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__StkSynthesizer__
#define __steeringwheel__StkSynthesizer__

#include <iostream>
#include <string>
#include <vector>

#include <RtAudio.h>
#include <JCRev.h>
#include <Voicer.h>

#include "sound/Synthesizer.h"

using namespace stk;

class StkSynthesizer : public Synthesizer {
public:
    StkSynthesizer(std::string rawwavePath, int channels = 2, double sampleRate = SRATE);
    virtual ~StkSynthesizer();
    
    void addInstrument(Instrmnt* instrument);
    
    virtual int getVoiceCount();
    
    void noteOn(double note, double amplitude) { Synthesizer::noteOn(note, amplitude); }
    void noteOff() { Synthesizer::noteOff(); }
    virtual void noteOn(double note, double amplitude, int voice);
    virtual void noteOff(int voice);
    
    virtual void start();
    virtual void stop();
    
private:
    int channels;
    RtAudio dac;
    
    std::vector<long> activeNote;
    int voiceCount;
    Voicer voicer;
    
    JCRev effect;
    
    static int tick(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *dataPointer);
};

#endif /* defined(__steeringwheel__StkSynthesizer__) */
