//
//  Piece.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Piece.h"

Piece::Piece(const std::string title, System& system, Wheeloin& instrument, Score& score) : Phase(system), instrument(instrument), scoreDisplay(*this, score), title(title) {
}

Piece::~Piece() {
}


void Piece::init() {
    Phase::init();
    scoreDisplay.init();
}

bool Piece::frame() {
    instrument.processInput();
    scoreDisplay.draw();
    return getTime() < scoreDisplay.getScore().getLength() + 5.0;
}

void Piece::setScore(Score& score) {
    scoreDisplay.setScore(score);
}

Wheeloin& Piece::getInstrument() {
    return instrument;
}

ScoreDisplay& Piece::getDisplay() {
    return scoreDisplay;
}

const std::string& Piece::getTitle() {
    return title;
}