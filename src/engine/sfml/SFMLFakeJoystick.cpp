//
//  SFMLFakeJoystick.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "SFMLFakeJoystick.h"
#include <SFML/Window/Keyboard.hpp>

SFMLFakeJoystick::SFMLFakeJoystick() {
    axes[0] = axes[1] = axes[2] = 0.0f;
    j2k[0] = sf::Keyboard::Q;
    j2k[1] = sf::Keyboard::W;
    j2k[8] = sf::Keyboard::D;
    j2k[9] = sf::Keyboard::E;
    
    for (std::map<int, sf::Keyboard::Key>::iterator i = j2k.begin(); i != j2k.end(); ++i) {
        k2j[i->second] = i->first;
    }
}

bool SFMLFakeJoystick::isButtonDown(int button) {
    return sf::Keyboard::isKeyPressed(mapJoystickButton(button));
}

void SFMLFakeJoystick::update() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        axes[1] -= 0.03;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        axes[1] += 0.03;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        axes[0] -= 0.01;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        axes[0] += 0.01;
    }
}

float SFMLFakeJoystick::getAxis(int axis) {
    return axes[axis];
}

int SFMLFakeJoystick::mapKeyboardButton(sf::Keyboard::Key key) {
    return k2j[key];
}

sf::Keyboard::Key SFMLFakeJoystick::mapJoystickButton(int button) {
    return j2k[button];
}