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

class RoundComputation : public Computation {
public:
    RoundComputation(System& system, int threads);
    void processChunk(int threadNumber, ComputationChunk& chunk, const Thread& thread);
protected:
    int getNumberOfElements();
    std::string getText(int count);
private:
    void processSubSeries(int nr, ComputationChunk& chunk, int baseProgress, const Thread& thread);
    
    void generateSequence(int s[3][12], int t[12]);
    double scoreTriad(int a, int b, int c);
    double score(int seq[3][12]);
    int count;
    
    double bestScore;
    int bestSeries[12];
};

#endif /* defined(__steeringwheel__RoundComputation__) */
