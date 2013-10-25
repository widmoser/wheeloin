//
//  SFMLSystem.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__SFMLSystem__
#define __steeringwheel__SFMLSystem__

#include <iostream>
#include <engine/System.h>
#include "SFMLKeyboard.h"
#include "SFMLJoystick.h"
#include "SFMLRenderer.h"

class SFMLSystem : public System {
public:
    SFMLSystem();
    SFMLSystem(int width, int height, bool fullscreen = false);
    virtual ~SFMLSystem();
    virtual Keyboard& getKeyboard();
    virtual Joystick& getJoystick();
    virtual Renderer& getRenderer();
    virtual void updateInput();
    virtual double getTime();
private:
    SFMLKeyboard keyboard;
    Joystick* joystick;
    SFMLRenderer renderer;
    
    bool fakeJoystick;
    sf::Clock clock;
};

#endif /* defined(__steeringwheel__SFMLSystem__) */
