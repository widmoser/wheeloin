//
//  SFMLSystem.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "SFMLSystem.h"

SFMLSystem::SFMLSystem() {
    
}

SFMLSystem::SFMLSystem(int width, int height, bool fullscreen) : renderer(width, height, fullscreen) {
    
}

SFMLSystem::~SFMLSystem() {
    
}

Keyboard& SFMLSystem::getKeyboard() {
    return keyboard;
}

Joystick& SFMLSystem::getJoystick() {
    return joystick;
}

Renderer& SFMLSystem::getRenderer() {
    return renderer;
}

void SFMLSystem::updateInput() {
    sf::Event event;
    while (renderer.getWindow().pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::KeyPressed) {
            keyboard.notifyButtonListenersDown(event.key.code);
        } else if (event.type == sf::Event::KeyReleased) {
            keyboard.notifyButtonListenersUp(event.key.code);
        } else if (event.type == sf::Event::JoystickButtonPressed) {
            joystick.notifyButtonListenersDown(event.joystickButton.button);
        } else if (event.type == sf::Event::JoystickButtonReleased) {
            joystick.notifyButtonListenersUp(event.joystickButton.button);
        }
    }
}

double SFMLSystem::getTime() {
    return clock.getElapsedTime().asSeconds();
}