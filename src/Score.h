//
//  Score.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/19/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Score__
#define __steeringwheel__Score__

#include <iostream>
#include <vector>
#include <string>

class Note {
public:
    Note(int voice, int value, double start, double length, double startVolume, double endVolume);
    Note() {}
    
    int voice;
    int value;
    double start;
    double length;
    double startVolume;
    double endVolume;
};

class Score {
public:
    Score(const std::string& filename);
    
    const std::vector<Note>& getNotes();
private:
    std::vector<Note> notes;
};

#endif /* defined(__steeringwheel__Score__) */
