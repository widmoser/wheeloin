//
//  Wheeloin.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Wheeloin__
#define __steeringwheel__Wheeloin__

#include <iostream>
#include <vector>
#include <cmath>
#include <engine/Keyboard.h>
#include <engine/System.h>
#include <sound/Synthesizer.h>
#include <Scale.h>
#include <Score.h>

class WheeloinConfiguration {
public:
    WheeloinConfiguration(const Scale& scale, int minNote, int maxNote) : scale(scale), minNote(minNote), maxNote(maxNote) {}
    
    int maxNote;
    int minNote;
    const Scale& scale;
};

class Wheeloin : public EventListener {
public:
    Wheeloin(Synthesizer& synth, System& system, WheeloinConfiguration configuration);
    
    void setTriggered(int voice, bool value = true);
    void onButtonDown(int button);
    void onButtonUp(int button);
    
    double getInputScaleNote() { return inputScaleNote; }
    int getInputNote() { return inputNote; }
    int getNote() { return note; }
    double getVolume() { return amplitude; }
    
    int getActiveVoice() { return synth.getActiveVoice(); }
    
    void processInput();
    
    WheeloinConfiguration& getConfiguration() { return conf; }
    
    
  
private:
    bool continuous;
    
    static const double e;
    
    Synthesizer& synth;
    System& system;
    
    Keyboard& keyboard;
    Joystick& joystick;
    
    double inputScaleNote;
    int inputNote;
    int note;
    double amplitude;
    double noteOff;
    
    double offset;
    
    WheeloinConfiguration conf;
    std::vector<bool> triggered;
};

#endif /* defined(__steeringwheel__Wheeloin__) */
