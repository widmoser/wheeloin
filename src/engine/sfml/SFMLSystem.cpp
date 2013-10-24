//
//  SFMLSystem.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "SFMLSystem.h"
#include <SFML/Window/Joystick.hpp>
#include "SFMLFakeJoystick.h"

SFMLSystem::SFMLSystem() {
    
}

SFMLSystem::SFMLSystem(int width, int height, bool fullscreen) : renderer(width, height, fullscreen) {
    if (sf::Joystick::isConnected(0)) {
        joystick = new SFMLJoystick();
        fakeJoystick = false;
    } else {
        joystick = new SFMLFakeJoystick();
        fakeJoystick = true;
    }
}

SFMLSystem::~SFMLSystem() {
    delete joystick;
}

Keyboard& SFMLSystem::getKeyboard() {
    return keyboard;
}

Joystick& SFMLSystem::getJoystick() {
    return *joystick;
}

Renderer& SFMLSystem::getRenderer() {
    return renderer;
}

void SFMLSystem::updateInput() {
    sf::Event event;
    keyboard.update();
    joystick->update();
    while (renderer.getWindow().pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::KeyPressed) {
            keyboard.notifyButtonListenersDown(event.key.code);
            if (fakeJoystick) {
                SFMLFakeJoystick* fj = dynamic_cast<SFMLFakeJoystick*>(joystick);
                joystick->notifyButtonListenersDown(fj->mapKeyboardButton(event.key.code));
            }
        } else if (event.type == sf::Event::KeyReleased) {
            keyboard.notifyButtonListenersUp(event.key.code);
            if (fakeJoystick) {
                SFMLFakeJoystick* fj = dynamic_cast<SFMLFakeJoystick*>(joystick);
                joystick->notifyButtonListenersUp(fj->mapKeyboardButton(event.key.code));
            }
        } else if (event.type == sf::Event::JoystickButtonPressed) {
            joystick->notifyButtonListenersDown(event.joystickButton.button);
        } else if (event.type == sf::Event::JoystickButtonReleased) {
            joystick->notifyButtonListenersUp(event.joystickButton.button);
        }
    }
}

double SFMLSystem::getTime() {
    return clock.getElapsedTime().asSeconds();
}