//
//  SFMLKeyboard.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__SFMLKeyboard__
#define __steeringwheel__SFMLKeyboard__

#include <iostream>
#include <engine/Keyboard.h>

class SFMLKeyboard : public Keyboard {
public:
    virtual bool isButtonDown(int button);
    virtual void update();
};

#endif /* defined(__steeringwheel__SFMLKeyboard__) */
