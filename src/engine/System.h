//
//  System.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__System__
#define __steeringwheel__System__

#include <iostream>
#include "Keyboard.h"
#include "Joystick.h"
#include "Renderer.h"

class System {
public:
    virtual Keyboard& getKeyboard() = 0;
    virtual Joystick& getJoystick() = 0;
    virtual Renderer& getRenderer() = 0;
    virtual void updateInput() = 0;
    
    virtual double getTime() = 0;
};

#endif /* defined(__steeringwheel__System__) */
