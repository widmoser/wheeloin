//
//  Joystick.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Joystick__
#define __steeringwheel__Joystick__

#include <iostream>
#include <vector>
#include "Keyboard.h"

class Joystick : public Keyboard {
    
public:
    
    virtual ~Joystick() = 0;
    
    virtual float getAxis(int axis) = 0;

};

#endif /* defined(__steeringwheel__Joystick__) */
