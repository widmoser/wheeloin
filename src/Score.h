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
#include <queue>
#include <string>

#include <engine/System.h>

class Note {
public:
    Note(int voice, int value, double start, double length, double startVolume, double endVolume);
    Note() : activated(false) {}
    
    int voice;
    int value;
    double start;
    double length;
    double startVolume;
    double endVolume;
    
    bool activated;
};

class Score {
public:
    Score();
    Score(const std::string& filename);
    
    void addNote(Note& n);
    
    const std::vector<Note>& getNotes();
    
    Note& nextNote();
    void popNote();
    bool noteLeft();
    
private:
    std::vector<Note> notes;
    int currentNote;
};

std::vector<Note> generateRound(System& system);

#endif /* defined(__steeringwheel__Score__) */
