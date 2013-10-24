//
//  SynthInstrument.h
//  SkiniPlayer
//
//  Created by Hannes Widmoser on 10/23/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __SkiniPlayer__SynthInstrument__
#define __SkiniPlayer__SynthInstrument__

#include <iostream>

#include <Instrmnt.h>
#include <Generator.h>

using namespace stk;

template <class G>
class SynthInstrument : public Instrmnt {
public:
    SynthInstrument();
    virtual ~SynthInstrument() {}
    
    virtual void setFrequency( StkFloat frequency );
    virtual void controlChange(int number, StkFloat value);
    virtual void noteOn( StkFloat frequency, StkFloat amplitude );
    virtual void noteOff( StkFloat amplitude );
    virtual StkFloat tick( unsigned int channel = 0 );
    virtual StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );
private:
    
    StkFloat getNote(StkFloat frequency);
    StkFloat getFrequency(StkFloat note);
    
    G generator;
};

StkFloat halfNote = 1.0 / 12.0;
StkFloat invPitch = 1.0 / 440.0;
StkFloat invln2 = 1.0 / log(2.0);

template <class G>
SynthInstrument<G>::SynthInstrument() {}

template <class G>
StkFloat SynthInstrument<G>::getNote(StkFloat frequency) {
    StkFloat result =  12*log(frequency*invPitch)*invln2 + 69;
    std::cout << frequency << " -> " << result << std::endl;
    return result;
}

template <class G>
StkFloat SynthInstrument<G>::getFrequency(StkFloat note) {
    StkFloat result = 440 * pow(2, (note - 69) * halfNote);
    std::cout << note << " -> " << result << std::endl;
    return result;
}

template <class G>
void SynthInstrument<G>::setFrequency( StkFloat frequency ) {
    generator.setFrequency(frequency);
}

template <class G>
void SynthInstrument<G>::controlChange(int number, StkFloat value) {
    // ignore for now
}

template <class G>
void SynthInstrument<G>::noteOn( StkFloat frequency, StkFloat amplitude ) {
    generator.setFrequency(frequency);
}

template <class G>
void SynthInstrument<G>::noteOff( StkFloat amplitude ) {
    
}

template <class G>
StkFloat SynthInstrument<G>::tick( unsigned int channel ) {
    lastFrame_[0] = generator.tick();
    return lastFrame_[0];
}

template <class G>
StkFrames& SynthInstrument<G>::tick( StkFrames& frames, unsigned int channel) {
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

#endif /* defined(__SkiniPlayer__SynthInstrument__) */
