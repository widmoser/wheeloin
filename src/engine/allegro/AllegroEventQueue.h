//
//  EventQueue.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__AllegroEventQueue__
#define __steeringwheel__AllegroEventQueue__

#include <iostream>
#include <allegro5/allegro.h>

class AllegroEventQueue {
public:
    AllegroEventQueue();
    ~AllegroEventQueue();
    
    void registerEventSource(ALLEGRO_EVENT_SOURCE* source);
    void unregisterEventSource(ALLEGRO_EVENT_SOURCE* source);
    ALLEGRO_EVENT getNextEvent();
private:
    ALLEGRO_EVENT_QUEUE* eventQueue;
};

#endif /* defined(__steeringwheel__AllegroEventQueue__) */
