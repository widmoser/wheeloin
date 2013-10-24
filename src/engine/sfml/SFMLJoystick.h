//
//  SFMLJoystick.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__SFMLJoystick__
#define __steeringwheel__SFMLJoystick__

#include <iostream>
#include <engine/Joystick.h>

class SFMLJoystick : public Joystick {
public:
    SFMLJoystick();
    virtual bool isButtonDown(int button);
    virtual void update();
    virtual float getAxis(int axis);
};

#endif /* defined(__steeringwheel__SFMLJoystick__) */
