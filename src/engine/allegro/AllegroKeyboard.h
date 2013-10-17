//
//  AllegroKeyboard.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__AllegroKeyboard__
#define __steeringwheel__AllegroKeyboard__

#include <iostream>
#include "engine/Keyboard.h"
#include "AllegroEventQueue.h"

class AllegroKeyboard : public Keyboard {
public:
    AllegroKeyboard(AllegroEventQueue& eventQueue);
    virtual ~AllegroKeyboard();
    
    virtual bool isButtonDown(int button);
    
    virtual void update();
    
private:
    ALLEGRO_KEYBOARD_STATE keyboardstate;
    AllegroEventQueue& eventQueue;
};

#endif /* defined(__steeringwheel__AllegroKeyboard__) */
