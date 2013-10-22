//
//  ScoreDisplay.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__ScoreDisplay__
#define __steeringwheel__ScoreDisplay__

#include <iostream>
#include <Wheeloin.h>
#include <Score.h>

class Piece;

class ScoreDisplay {
public:
    ScoreDisplay(Piece& piece, Score& score);
    void draw();
    void init();
private:
    float getAspect();
    
    void setProjection();
    void setCamera(float note, float time);
    void setView();
    
    void processInput();
    
    void clear();
    
    void drawGrid();
    void drawNotes();
    void drawCursor();
    void drawTextOverlay(float delta);
    
    Wheeloin& instrument;
    Renderer& renderer;
    Score& score;
    System& system;
    Piece& piece;
    
    float position;
    float gridWidth;
    float gridLength;
};

#endif /* defined(__steeringwheel__ScoreDisplay__) */
