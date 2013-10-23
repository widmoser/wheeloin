//
//  Pause.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Pause__
#define __steeringwheel__Pause__

#include <iostream>
#include <engine/System.h>
#include <engine/Phase.h>

class Pause : public Phase {
public:
    Pause(System& system);
    void init();
    bool frame();
private:
    System& system;
};
#endif /* defined(__steeringwheel__Pause__) */
