//
//  RoundComputation.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__RoundComputation__
#define __steeringwheel__RoundComputation__

#include <iostream>
#include <sstream>
#include <Computation.h>

class Parameters {
public:
    Parameters(int voiceCount, double offset, double noteLengths[12]);
    ~Parameters();
    
    int** allocateEmptyScore();
    
    double* noteLengths;
    
    int offset;
    int voiceCount;
    double tickLength;
    int tickCount;
    int* ticks;
};

class SkiniNote {
public:
    double time;
    int voice;
    int note;
};

class RoundComputation : public Computation {
public:
    RoundComputation(System& system, Parameters& parameters, int threads);
    void processChunk(int threadNumber, ComputationChunk& chunk, const Thread& thread);
    void finalize();
protected:
    int getNumberOfElements();
    std::string getText(int count);
private:
    void processSubSeries(int nr, ComputationChunk& chunk, int baseProgress, const Thread& thread);
    
    void printNoteOff(std::ostream& out, double delta, int voice, int note);
    void printNote(std::ostream& out, double delta, int voice, int note);
    
    void fillSequence(int* seq, int t[12], int offset);
    void fillScore(int** score, int t[12]);
    double scoreTriad(int a, int b, int c);
    double score(int** score);
    int count;
    
    Parameters& parameters;
    
    double bestScore;
    int bestSeries[12];
};

#endif /* defined(__steeringwheel__RoundComputation__) */
