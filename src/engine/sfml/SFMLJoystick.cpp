//
//  SFMLJoystick.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "SFMLJoystick.h"
#include <SFML/Window/Joystick.hpp>

bool SFMLJoystick::isButtonDown(int button) {
    return sf::Joystick::isButtonPressed(0, button);
}

void SFMLJoystick::update() {
    sf::Joystick::update();
}

float SFMLJoystick::getAxis(int axis) {
    return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis(axis))*0.01f;
}