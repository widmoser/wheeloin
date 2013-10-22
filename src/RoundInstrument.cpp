//
//  RoundInstrument.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "RoundInstrument.h"

RoundInstrument::RoundInstrument(Instrmnt& delegate) : delegate(delegate), delay1(44100*4, 44100*4), delay2(44100*8, 44100*8) {
}

void RoundInstrument::setFrequency(StkFloat frequency) {
    delegate.setFrequency(frequency);
}

void RoundInstrument::controlChange(int number, StkFloat value) {
    delegate.controlChange(number, value);
}

void RoundInstrument::noteOn(StkFloat frequency, StkFloat amplitude) {
    delegate.noteOn(frequency, amplitude);
}

void RoundInstrument::noteOff(StkFloat amplitude) {
    delegate.noteOff(amplitude);
}

StkFloat RoundInstrument::tick(unsigned int channel) {
    for (int i = 0; i < lastFrame_.channels(); ++i) {
        StkFloat a = delegate.tick(i);
        StkFloat b = delay1.tick(a);
        StkFloat c = delay2.tick(a);
        lastFrame_[i] = 0.75*(a + b + c);
    }
    return lastFrame_[channel];
}

StkFrames& RoundInstrument::tick(StkFrames& frames, unsigned int channel) {
    unsigned int nChannels = lastFrame_.channels();
#if defined(_STK_DEBUG_)
    if ( channel > frames.channels() - nChannels ) {
        oStream_ << "VoicForm::tick(): channel and StkFrames arguments are incompatible!";
        handleError( StkError::FUNCTION_ARGUMENT );
    }
#endif
    
    StkFloat *samples = &frames[channel];
    unsigned int j, hop = frames.channels() - nChannels;
    if ( nChannels == 1 ) {
        for ( unsigned int i=0; i<frames.frames(); i++, samples += hop )
            *samples++ = tick();
    }
    else {
        for ( unsigned int i=0; i<frames.frames(); i++, samples += hop ) {
            *samples++ = tick();
            for ( j=1; j<nChannels; j++ )
                *samples++ = lastFrame_[j];
        }
    }
    
    return frames;
}