//
//  SFMLFakeJoystick.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__SFMLFakeJoystick__
#define __steeringwheel__SFMLFakeJoystick__

#include <iostream>
#include <engine/Joystick.h>
#include <SFML/Window/Keyboard.hpp>
#include <map>

class SFMLFakeJoystick : public Joystick {
public:
    SFMLFakeJoystick();
    virtual bool isButtonDown(int button);
    virtual void update();
    virtual float getAxis(int axis);
    
    int mapKeyboardButton(sf::Keyboard::Key key);
    sf::Keyboard::Key mapJoystickButton(int button);
private:
    std::map<int, sf::Keyboard::Key> j2k;
    std::map<sf::Keyboard::Key, int> k2j;
    float axes[3];
};

#endif /* defined(__steeringwheel__SFMLFakeJoystick__) */
