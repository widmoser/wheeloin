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
#include <vector>
#include <queue>
#include <Computation.h>
#include <Score.h>
#include <Piece.h>

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
    double* tickWeight;
};

class SkiniNote {
public:
    double time;
    int voice;
    int note;
};

enum Chords {
    PURE = 0,
    FOURTH = 1,
    MAJ = 2,
    MIN = 3,
    MAJ_7 = 4,
    MIN_7 = 5,
    DIM = 6,
    PURE_7 = 7,
    DIM_7 = 8,
    AUG = 9,
    MAJ_M7 = 10,
    MIN_M7 = 11,
    DIM_M7 = 12,
    PURE_M7 = 13,
    SOFT_CLUSTER = 14,
    HARD_CLUSTER = 15
};

class Series {
public:
    Series() : score(0.0) {
        std::fill_n(data, 12, 0);
    }
    
    Series(const Series& s) : score(s.score) {
        memcpy(data, s.data, sizeof(int)*12);
    }
    
    double score;
    int data[12];
    
    bool operator<(const Series& s) const {
        return score > s.score;
    }
    
    void operator=(const Series& s) {
        score = s.score;
        memcpy(data, s.data, sizeof(int)*12);
    }
};

class RoundComputation : public Computation {
public:
    RoundComputation(System& system, Parameters& parameters, int threads, Piece& next);
    ~RoundComputation();
    void processChunk(int threadNumber, ComputationChunk& chunk, const Thread& thread);
    void init();
    void finalize();
    
    Score& getScore();
    
protected:
    int getNumberOfElements();
    std::string getText(int count);
private:
    void processSubSeries(int nr, ComputationChunk& chunk, int baseProgress, const Thread& thread, std::priority_queue<Series>& bestSeries);
    
    void printNoteOff(std::ostream& out, double delta, int voice, int note);
    void printNote(std::ostream& out, double delta, int voice, int note);
    
    int& chord(int i, int j);
    void setChord(int ch, int i, int j);
    void initializeChord(int chord, int i, int j);
    void initializeChords();
    
    void fillSequence(int* seq, int t[12], int offset);
    void fillScore(int** score, int t[12]);
    int getChord(int a, int b, int c, int& d1, int& d2);
    double score(int** score);
    int count;
    
    Series popMin();
    void merge();
    
    Parameters& parameters;
    
    std::vector<std::priority_queue<Series> > queues;
    std::vector<Series> result;
    int* chords;
    
    Score bestScore;
    Piece& next;
};

#endif /* defined(__steeringwheel__RoundComputation__) */
