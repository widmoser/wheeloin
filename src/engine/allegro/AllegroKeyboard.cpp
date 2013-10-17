//
//  AllegroKeyboard.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include <allegro5/allegro.h>
#include "AllegroKeyboard.h"
#include "Exception.h"

AllegroKeyboard::AllegroKeyboard(AllegroEventQueue& eventQueue) : eventQueue(eventQueue) {
    if (!al_install_keyboard()) {
        throw Exception("Could not install keyboard");
    }
    eventQueue.registerEventSource(al_get_keyboard_event_source());
}

AllegroKeyboard::~AllegroKeyboard() {
    eventQueue.unregisterEventSource(al_get_keyboard_event_source());
    al_uninstall_keyboard();
}

bool AllegroKeyboard::isButtonDown(int button) {
    return al_key_down(&keyboardstate, button);
}

void AllegroKeyboard::update() {
    al_get_keyboard_state(&keyboardstate);
    
}