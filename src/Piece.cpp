//
//  Piece.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Piece.h"

Piece::Piece(System& system, Score& score) : wheeloin(synth, system, WheeloinConfiguration(Scales::MAJOR, 15, 54), score), scoreDisplay(wheeloin, system, score) {
    synth.start();
}

Piece::~Piece() {
    synth.stop();
}


void Piece::init() {
    scoreDisplay.init();
}

bool Piece::frame() {
    wheeloin.processInput();
    scoreDisplay.draw();
    return true;
}