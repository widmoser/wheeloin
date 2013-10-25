//
//  Renderer.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Renderer__
#define __steeringwheel__Renderer__

#include <iostream>
#include <string>

class Renderer {
public:
    
    virtual void setTextColor(int r, int g, int b) = 0;
    virtual void drawText(float x, float y, const std::string& text, int size) = 0;
    
    virtual int getDisplayWidth() = 0;
    virtual int getDisplayHeight() = 0;
    
    virtual void updateDisplay() = 0;
};

#endif /* defined(__steeringwheel__Renderer__) */
