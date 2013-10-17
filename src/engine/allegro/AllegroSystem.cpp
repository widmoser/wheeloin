//
//  AllegroSystem.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "AllegroSystem.h"

AllegroSystem::AllegroSystem() {
    al_init();
    eventQueue = new AllegroEventQueue();
    keyboard = new AllegroKeyboard(*eventQueue);
    joystick = new AllegroJoystick(*eventQueue);
}

AllegroSystem::~AllegroSystem() {
    delete keyboard;
    delete joystick;
    delete eventQueue;
    al_uninstall_system();
}

Keyboard& AllegroSystem::getKeyboard() {
    return *keyboard;
}

Joystick& AllegroSystem::getJoystick() {
    return *joystick;
}

void AllegroSystem::updateInput() {
    keyboard->update();
    joystick->update();
    ALLEGRO_EVENT event = eventQueue->getNextEvent();
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        keyboard->notifyButtonListeners(event.keyboard.keycode);
    } else if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
        joystick->notifyButtonListeners(event.joystick.button);
    }
}

double AllegroSystem::getTime() {
    return al_get_time();
}