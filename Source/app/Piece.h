//
//  Piece.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Piece__
#define __steeringwheel__Piece__

#include <iostream>
#include <engine/Phase.h>
#include <engine/System.h>
#include <Score.h>
#include <Wheeloin.h>
#include <WheeloinSynth.h>
#include <ScoreDisplay.h>

class Piece : public Phase {
public:
    Piece(const std::string title, System& system, Wheeloin& instrument, Score& score);
    ~Piece();
    void init();
    virtual bool frame();
    
    void setScore(Score& score);
    
    Wheeloin& getInstrument();
    ScoreDisplay& getDisplay();
    
    const std::string& getTitle();
private:
    Wheeloin& instrument;
    ScoreDisplay scoreDisplay;
    std::string title;
};

#endif /* defined(__steeringwheel__Piece__) */
