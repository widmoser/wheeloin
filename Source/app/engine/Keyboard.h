//
//  Keyboard.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Keyboard__
#define __steeringwheel__Keyboard__

#include <iostream>
#include <vector>

class EventListener {
public:
    virtual void onButtonDown(int button) = 0;
    virtual void onButtonUp(int button) = 0;
};

class Keyboard {
    
public:
    
    virtual bool isButtonDown(int button) = 0;
    
    virtual void update() = 0;
    
    virtual void registerButtonListener(EventListener* listener);
    
    virtual void unregisterButtonListener(EventListener* listener);
    
    void notifyButtonListenersDown(int button);
    void notifyButtonListenersUp(int button);
private:
    
    std::vector<EventListener*> listeners;
    
};

#endif /* defined(__steeringwheel__Keyboard__) */
