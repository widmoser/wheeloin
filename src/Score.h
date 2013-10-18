//
//  Score.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Score__
#define __steeringwheel__Score__

#include <iostream>
#include <Wheeloin.h>

class Score {
public:
    Score(Wheeloin& instrument, Renderer& renderer);
    void draw();
private:
    
    void drawTextOverlay();
    void setupOpenGlMatrices(float note, float time);
    
    Wheeloin& instrument;
    Renderer& renderer;
    
    float position;
};

#endif /* defined(__steeringwheel__Score__) */
