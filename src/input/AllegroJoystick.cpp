//
//  AllegroJoystick.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "input/AllegroJoystick.h"
#include <allegro5/allegro.h>
#include "Exception.h"

AllegroJoystick::AllegroJoystick() {
    if (!al_install_joystick())
        std::cerr << "ERROR: could not initialize input device driver" << std::endl;
    if (al_get_num_joysticks() <= 0)
        std::cerr << "ERROR: no input device found" << std::endl;
    input_device = al_get_joystick(0);
    if (!input_device) {
        throw Exception("Could not retrieve joystick");
    }
    eventQueue.registerEventSource(al_get_joystick_event_source());
}

AllegroJoystick::~AllegroJoystick() {
    eventQueue.unregisterEventSource(al_get_joystick_event_source());
    al_release_joystick(input_device);
    al_uninstall_joystick();
}

bool AllegroJoystick::isButtonDown(int button) {
    return state.button[button];
}

void AllegroJoystick::update() {
    al_get_joystick_state(input_device, &state);
    ALLEGRO_EVENT event = eventQueue.getNextEvent();
    if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
        notifyButtonListeners(event.joystick.button);
    }
}

float AllegroJoystick::getAxis(int axis) {
    return state.stick->axis[axis];
}