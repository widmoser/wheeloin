//
//  Synthesizer.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Synthesizer__
#define __steeringwheel__Synthesizer__

#include <iostream>

class Synthesizer {
public:
    Synthesizer();
    
    int getActiveVoice();
    void setActiveVoice(int voice);
    
    virtual int getVoiceCount() = 0;
    
    void noteOn(double note, double amplitude);
    void noteOff();
    bool isNoteOn();
    double getNote();
    
    //virtual void setFrequency(double note) = 0;
    virtual void noteOn(double note, double amplitude, int voice) = 0;
    virtual void noteOff(int voice) = 0;
    virtual bool isNoteOn(int voice) = 0;
    virtual double getNote(int voice) = 0;
    
    virtual void start() = 0;
    virtual void stop() = 0;
    
private:
    int activeVoice;
};

#endif /* defined(__steeringwheel__Synthesizer__) */
