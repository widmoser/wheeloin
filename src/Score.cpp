//
//  Score.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/19/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Score.h"
#include <Exception.h>
#include <fstream>

Note::Note(int voice, int value, double start, double length, double startVolume, double endVolume) : voice(voice), value(value), start(start), length(length), startVolume(startVolume), endVolume(endVolume) {
}

Score::Score(const std::string& filename) : currentNote(0) {
    std::ifstream input(filename.c_str());
    if (input.fail()) {
        throw Exception("Could not open file " + filename);
    }
    Note n;
    while (!input.eof()) {
        input >> n.start >> n.length >> n.voice >> n.value >> n.startVolume >> n.endVolume;
        notes.push_back(n);
    }
}

const std::vector<Note>& Score::getNotes() {
    return notes;
}

Note& Score::nextNote() {
    std::cout << currentNote << std::endl;
    return notes[currentNote];
}

void Score::popNote() {
    currentNote++;
}


bool Score::noteLeft() {
    return currentNote < notes.size();
}