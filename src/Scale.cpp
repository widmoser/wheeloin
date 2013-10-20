//
//  Scale.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Scale.h"
#include <cmath>

std::string names[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "H"
};

std::string getNoteName(int note) {
    if (note > 4) {
        return names[(note-4) % 12];
    } else {
        return "";
    }
}

Scale::Scale(int start) {
    data.push_back(start);
}

int Scale::getNote(double scalenote) const {
    // rawnote is in terms of scale:
    int rounded = int(round(scalenote));
    long octave = rounded / data.size();
    long note = rounded % data.size();
    return int(octave*12 + data[note]);
}

int Scale::getScaleNote(double note, bool& accidental) const {
    int intNote = int(round(note));
    int relNote = intNote % 12;
    int octave = intNote / 12;
    for (std::vector<int>::const_iterator i = data.begin(); i != data.end(); ++i) {
        if (*i > relNote) {
            accidental = *(i-1) != relNote;
            return int(octave*data.size() + (i-1-data.begin()));
        }
    }
    accidental = *(data.end()-1) != relNote;
    return int(octave*data.size() + (data.size() - 1));
}

Scale& Scale::operator|(int arg) {
    data.push_back(arg);
    return *this;
}

Scale Scales::_0 = Scale();

Scale Scales::MAJOR = _0 | 2 | 4 | 5 | 7 | 9 | 11;