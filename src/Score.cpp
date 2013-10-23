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
#include <algorithm>


#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

Note::Note(int voice, int value, double start, double length, double startVolume, double endVolume) : voice(voice), value(value), start(start), length(length), startVolume(startVolume), endVolume(endVolume), activated(false) {
}

Score::Score() : currentNote(0) {
    
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

void Score::addNote(Note& n) {
    notes.push_back(n);
}

const std::vector<Note>& Score::getNotes() {
    return notes;
}

Note& Score::nextNote() {
    std::cout << currentNote << std::endl;
    return notes[currentNote];
}

bool Score::hasNextNote() {
    return currentNote < notes.size();
}

void Score::popNote() {
    currentNote++;
}


bool Score::noteLeft() {
    return currentNote < notes.size();
}

double Score::getLength() {
    if (notes.size() > 0) {
        return notes.back().start + notes.back().length;
    } else {
        return 0.0;
    }
}