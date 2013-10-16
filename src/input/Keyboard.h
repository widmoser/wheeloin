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

typedef void (*ButtonListener)(int);

class Keyboard {
    
public:
    
    virtual bool isButtonDown(int button) = 0;
    
    virtual void update() = 0;
    
    virtual void registerButtonListener(ButtonListener listener);
    
    virtual void unregisterButtonListener(ButtonListener listener);
    
    void notifyButtonListeners(int button);
    
private:
    
    std::vector<ButtonListener> listeners;
    
};

#endif /* defined(__steeringwheel__Keyboard__) */
