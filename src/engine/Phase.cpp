//
//  Phase.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Phase.h"

Phase::Phase(System& system) : system(system), startTime(-1.0) {
    
}

void Phase::init() {
    startTime = system.getTime();
}

double Phase::getTime() {
    if (startTime < 0.0) {
        return 0.0;
    } else {
        return system.getTime() - startTime;
    }
}

System& Phase::getSystem() {
    return system;
}