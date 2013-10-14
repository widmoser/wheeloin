//
//  VolumeSine.h
//  Test
//
//  Created by Hannes Widmoser on 7/7/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __Test__VolumeSine__
#define __Test__VolumeSine__

#include <iostream>
#include "SineWave.h"

using namespace stk;

class VolumeSineWave : public SineWave {
    
public:
    VolumeSineWave() : nextVolume(1.0), prevVolume(1.0), volume(1.0) {}
    
    StkFloat tick();
    
    void setVolume(StkFloat newVolume);
    
protected:
    StkFloat nextVolume;
    StkFloat prevVolume;
    StkFloat volume;
};

#endif /* defined(__Test__VolumeSine__) */
