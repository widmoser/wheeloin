//
//  WheeloinSynth.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__WheeloinSynth__
#define __steeringwheel__WheeloinSynth__

#include <iostream>
#include <sound/stk/StkSynthesizer.h>

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
#include <RoundInstrument.h>
#include "Overdrive.h"

class WheeloinSynth : public StkSynthesizer {
public:
    WheeloinSynth();
private:
    HevyMetl heavyMetl;
    Whistle whistle;
    VoicForm voices;
    VoicForm sing;
    FMVoices fmVoices;
    FMVoices fmVoices1;
    FMVoices fmVoices2;
    Rhodey rhodey;
    TubeBell bell;
    
    Overdrive ov;
    Overdrive ov2;
    
    SampleInstrument sample;
    SampleInstrument sample2;
    SampleInstrument sample3;
    SampleInstrument sample4;
    
    RoundInstrument round;
    
};

#endif /* defined(__steeringwheel__WheeloinSynth__) */
