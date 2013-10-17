//
//  AllegroEventQueue.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "AllegroEventQueue.h"
#include "Exception.h"

AllegroEventQueue::AllegroEventQueue() {
    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        throw Exception("Failed to create event queue");
    }
}

AllegroEventQueue::~AllegroEventQueue() {
    al_destroy_event_queue(eventQueue);
}

void AllegroEventQueue::registerEventSource(ALLEGRO_EVENT_SOURCE* source) {
    al_register_event_source(eventQueue, source);
}

void AllegroEventQueue::unregisterEventSource(ALLEGRO_EVENT_SOURCE* source) {
    al_unregister_event_source(eventQueue, source);
}

ALLEGRO_EVENT AllegroEventQueue::getNextEvent() {
    ALLEGRO_EVENT event;
    al_get_next_event(eventQueue, &event);
    return event;
}