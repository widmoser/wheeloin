//
//  WheeloinSynth.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "WheeloinSynth.h"

WheeloinSynth::WheeloinSynth() : StkSynthesizer("./rawwaves"), sample("./samples/klanginstallation/bg - vogel sehr langsam.wav"), sample2("./samples/BASSLINES_128bpm/Bass_01_C.wav"), sample3("./samples/BASSLINES_128bpm/Bass_02_D#.wav"), sample4("./samples/BASSLINES_128bpm/Bass_03_E.wav"), ov(&heavyMetl), ov2(&whistle), round(fmVoices) {
    whistle.controlChange(4, 10.0);
    ov.setOverdrive(5.0);
    
    
    addInstrument(&fmVoices);
    addInstrument(&round);
    addInstrument(&fmVoices2);
    addInstrument(&ov);
    addInstrument(&sample);
}
