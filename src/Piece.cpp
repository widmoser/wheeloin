//
//  Piece.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Piece.h"

Piece::Piece(System& system, Score& score) : Phase(system), wheeloin(synth, system, WheeloinConfiguration(Scales::MAJOR, 15, 54), score), scoreDisplay(*this, score) {
    synth.start();
}

Piece::~Piece() {
    synth.stop();
}


void Piece::init() {
    Phase::init();
    scoreDisplay.init();
}

bool Piece::frame() {
    wheeloin.processInput();
    scoreDisplay.draw();
    return true;
}

WheeloinSynth& Piece::getSynth() {
    return synth;
}

Wheeloin& Piece::getInstrument() {
    return wheeloin;
}

ScoreDisplay& Piece::getDisplay() {
    return scoreDisplay;
}