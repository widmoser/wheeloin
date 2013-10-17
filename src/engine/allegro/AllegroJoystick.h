//
//  AllegroJoystick.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__AllegroJoystick__
#define __steeringwheel__AllegroJoystick__

#include <iostream>
#include "engine/Joystick.h"
#include "AllegroEventQueue.h"

class AllegroJoystick : public Joystick {
public:
    AllegroJoystick(AllegroEventQueue& eventQueue);
    virtual ~AllegroJoystick();
    
    virtual bool isButtonDown(int button);
    
    virtual void update();
    
    virtual float getAxis(int axis);

private:
    ALLEGRO_JOYSTICK* input_device;
    ALLEGRO_JOYSTICK_STATE state;
    AllegroEventQueue& eventQueue;
};

#endif /* defined(__steeringwheel__AllegroJoystick__) */
