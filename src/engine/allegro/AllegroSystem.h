//
//  AllegroSystem.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__AllegroSystem__
#define __steeringwheel__AllegroSystem__

#include <iostream>
#include "engine/System.h"
#include "AllegroKeyboard.h"
#include "AllegroJoystick.h"
#include "AllegroRenderer.h"

class AllegroSystem : public System {
public:
    AllegroSystem();
    AllegroSystem(int width, int height, bool fullscreen = false);
    virtual ~AllegroSystem();
    virtual Keyboard& getKeyboard();
    virtual Joystick& getJoystick();
    virtual Renderer& getRenderer();
    virtual void updateInput();
    virtual double getTime();
private:
    void initInput();
    void initFont();
    
    AllegroKeyboard* keyboard;
    AllegroJoystick* joystick;
    AllegroEventQueue* eventQueue;
    AllegroRenderer* renderer;
};

#endif /* defined(__steeringwheel__AllegroSystem__) */
