//
//  RoundInstrument.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__RoundInstrument__
#define __steeringwheel__RoundInstrument__

#include <iostream>
#include <Instrmnt.h>
#include <Delay.h>

using namespace stk;

class RoundInstrument : public Instrmnt {
public:
    RoundInstrument(Instrmnt& delegate);
    virtual ~RoundInstrument() {}
    
    virtual void setFrequency( StkFloat frequency );
    virtual void controlChange(int number, StkFloat value);
    virtual void noteOn( StkFloat frequency, StkFloat amplitude );
    virtual void noteOff( StkFloat amplitude );
    virtual StkFloat tick( unsigned int channel = 0 );
    virtual StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );
private:
    Instrmnt& delegate;
    Delay delay1;
    Delay delay2;
};

#endif /* defined(__steeringwheel__RoundInstrument__) */
