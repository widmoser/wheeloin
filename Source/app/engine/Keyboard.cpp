//
//  Keyboard.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Keyboard.h"

void Keyboard::registerButtonListener(EventListener* listener) {
    listeners.push_back(listener);
}

void Keyboard::unregisterButtonListener(EventListener* listener) {
    listeners.erase(std::find(listeners.begin(), listeners.end(), listener));
}

void Keyboard::notifyButtonListenersDown(int button) {
    for (std::vector<EventListener*>::iterator i = listeners.begin(); i != listeners.end(); ++i) {
        (*i)->onButtonDown(button);
    }
}

void Keyboard::notifyButtonListenersUp(int button) {
    for (std::vector<EventListener*>::iterator i = listeners.begin(); i != listeners.end(); ++i) {
        (*i)->onButtonUp(button);
    }
}