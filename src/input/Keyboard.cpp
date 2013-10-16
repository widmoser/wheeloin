//
//  Keyboard.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Keyboard.h"

void Keyboard::registerButtonListener(ButtonListener* listener) {
    listeners.push_back(listener);
}

void Keyboard::unregisterButtonListener(ButtonListener* listener) {
    listeners.erase(std::find(listeners.begin(), listeners.end(), listener));
}

void Keyboard::notifyButtonListeners(int button) {
    for (std::vector<ButtonListener*>::iterator i = listeners.begin(); i != listeners.end(); ++i) {
        (*i)->onButtonDown(button);
    }
}