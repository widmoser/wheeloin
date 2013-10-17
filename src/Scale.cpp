//
//  Scale.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Scale.h"
#include <cmath>

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

Scale& Scale::operator|(int arg) {
    data.push_back(arg);
    return *this;
}

Scale Scales::_0 = Scale();

Scale Scales::MAJOR = _0 | 2 | 4 | 5 | 7 | 9 | 11;