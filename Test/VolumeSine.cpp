//
//  VolumeSine.cpp
//  Test
//
//  Created by Hannes Widmoser on 7/7/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "VolumeSine.h"

#include <iostream>
#include <cmath>


StkFloat VolumeSineWave::tick() {
    if (fabs(nextVolume - volume) > 0.001)
        volume += (nextVolume - prevVolume)*0.001;
    
    return SineWave::tick() * volume;
}

void VolumeSineWave::setVolume(StkFloat newVolume) {
    prevVolume = volume;
    nextVolume = newVolume;
    std::cout << nextVolume << " " << prevVolume << " " << volume << std::endl;
}
