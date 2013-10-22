//
//  RoundComputation.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "RoundComputation.h"

RoundComputation::RoundComputation(Renderer& renderer, int threads) : Computation(renderer, threads) {
}

double rating[12] = {
    2.0, 0.0, 1.0, 3.0, 3.0, 2.0, 1.0, 2.0, 3.0, 3.0, 1.0, 0.0
};

int MAX_COUNT = 479001600; // 12!
//int MAX_COUNT = 3000000;

int MAX_SUB_COUNT = 39916800;

void RoundComputation::generateSequence(int s[3][12], int t[12]) {
    memcpy(s[0], t, 12*sizeof(int));
    
    memcpy(s[1], t+10, 2*sizeof(int));
    memcpy(s[1]+2, t, 10*sizeof(int));
    
    memcpy(s[2], t+8, 4*sizeof(int));
    memcpy(s[2]+4, t, 8*sizeof(int));
}

double RoundComputation::scoreTriad(int a, int b, int c) {
    // should sound imperfect:
    int d1 = abs(a - b) % 12;
    int d2 = abs(a - c) % 12;
    int d3 = abs(b - c) % 12;
    
    return rating[d1] + rating[d2] + rating[d3];
}

double RoundComputation::score(int seq[3][12]) {
    double s = 0.0;
    for (int i = 0; i < 12; ++i) {
        s += scoreTriad(seq[0][i], seq[1][i], seq[2][i]);
    }
    return s;
}

void RoundComputation::processSubSeries(int nr, ComputationChunk& chunk, int baseProgress) {
    int s[3][12];
    int series[12];
    series[0] = nr;
    for (int i = 1; i < nr; ++i) {
        series[i] = i;
    }
    for (int i = nr; i < 12; ++i) {
        series[i] = i+1;
    }
    int c = 0;
    do {
        generateSequence(s, series);
        
        double sc = score(s);
        if (sc > bestScore) {
            bestScore = sc;
            memcpy(bestSeries, series, 12*sizeof(int));
        }
        
        if (c++ % 100000 == 0) {
            // report progress:
            chunk.setProgress(baseProgress + c);
        }
    } while (std::next_permutation(series+1, series+12));
    count += c;
    std::stringstream txt;
    txt << "Processing twelve tone series: " << count/1000000 << " Mio.";
    setText(txt.str());
}

void RoundComputation::processChunk(int thread, ComputationChunk& chunk) {
    for (int i = 0; i < 12; ++i) {
        if (i % threadCount == thread) {
            processSubSeries(i, chunk, chunk.getProgress());
        }
    }
}