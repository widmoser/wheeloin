//
//  AllegroSystem.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/17/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "AllegroSystem.h"

void AllegroSystem::initInput() {
    eventQueue = new AllegroEventQueue();
    keyboard = new AllegroKeyboard(*eventQueue);
    joystick = new AllegroJoystick(*eventQueue);
}

AllegroSystem::AllegroSystem() {
    al_init();
    initInput();
    renderer = new AllegroRenderer();
}

AllegroSystem::AllegroSystem(int width, int height, bool fullscreen) {
    al_init();
    initInput();
    renderer = new AllegroRenderer(width, height, fullscreen);
}

AllegroSystem::~AllegroSystem() {
    delete renderer;
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

Renderer& AllegroSystem::getRenderer() {
    return *renderer;
}

void AllegroSystem::updateInput() {
    keyboard->update();
    joystick->update();
    ALLEGRO_EVENT event = eventQueue->getNextEvent();
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        keyboard->notifyButtonListenersDown(event.keyboard.keycode);
    } else if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
        joystick->notifyButtonListenersDown(event.joystick.button);
    } else if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
        joystick->notifyButtonListenersUp(event.joystick.button);
    }
}

double AllegroSystem::getTime() {
    return al_get_time();
}