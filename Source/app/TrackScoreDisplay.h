//
//  TrackScoreDisplay.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/19/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__TrackScoreDisplay__
#define __steeringwheel__TrackScoreDisplay__

#include <iostream>
#include <Wheeloin.h>
#include <Score.h>
#include <Camera.h>

class TrackScoreDisplay {
public:
    TrackScoreDisplay(Wheeloin& instrument, System& system, Score& score);
    void draw();
private:
    float getAspect();
    
    void setProjection();
    void setCamera(float note, float time, float delta);
    void setView(float delta);
    
    void clear();
    
    void drawGrid();
    void drawNotes();
    void drawCursor();
    void drawTextOverlay(float delta);
    
    float getAngle(int note);
    
    
    Wheeloin& instrument;
    System& system;
    Renderer& renderer;
    Score& score;
    
    float position;
    float gridWidth;
    float gridLength;
    
    Camera camera;
};

#endif /* defined(__steeringwheel__TrackScoreDisplay__) */
