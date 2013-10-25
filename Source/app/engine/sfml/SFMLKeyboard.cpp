//
//  SFMLKeyboard.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "SFMLKeyboard.h"
#include <SFML/Window/Keyboard.hpp>

bool SFMLKeyboard::isButtonDown(int button) {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Key(button));
}

void SFMLKeyboard::update() {
    // not necessary?
}