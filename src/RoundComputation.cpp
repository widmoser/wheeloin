//
//  RoundComputation.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "RoundComputation.h"
#include <limits>
#include <fstream>

Parameters::Parameters(int voiceCount, double offset, double noteLengths[12]) : voiceCount(voiceCount), noteLengths(noteLengths) {
    double minNoteLength = std::numeric_limits<double>::max();
    double overallLength = 0.0;
    for (int i = 0; i < 12; ++i) {
        double len = noteLengths[i];
        if (len < minNoteLength) {
            minNoteLength = len;
        }
        overallLength += len;
    }
    
    tickCount = int(overallLength / minNoteLength);
    tickLength = minNoteLength;
    ticks = new int[tickCount];
    int j = 0;
    double currentLength = noteLengths[j];
    for (int i = 0; i < tickCount; ++i) {
        ticks[i] = j;
        currentLength -= minNoteLength;
        if (currentLength <= 0) {
            j++;
            currentLength = noteLengths[j];
        }
    }
    
    this->offset = int(offset / minNoteLength);
}

Parameters::~Parameters() {
    delete ticks;
}

int** Parameters::allocateEmptyScore() {
    int** res = new int*[voiceCount];
    for (int i = 0; i < voiceCount; ++i) {
        res[i] = new int[tickCount];
    }
    return res;
}

const int REPORT_PROGRESS_COUNT = 100000;

RoundComputation::RoundComputation(System& system, Parameters& parameters, int threads) : Computation(system, threads), parameters(parameters) {
}

void RoundComputation::printNoteOff(std::ostream& out, double delta, int voice, int note) {
    out << "NoteOff " << delta << " " << voice << " " << note << " 100" << std::endl;
}

void RoundComputation::printNote(std::ostream& out, double delta, int voice, int note) {
    out << "NoteOn " << delta << " " << voice << " " << note << " 100" << std::endl;
}

bool compareNotes(SkiniNote a, SkiniNote b) {
    return a.time < b.time;
}

void RoundComputation::finalize() {
    Computation::finalize();
    
    for (int i = 0; i < 12; ++i) {
        std::cout << bestSeries[i] << " ";
    }
    std::cout << std::endl;
    
    double timeOffset = parameters.offset*parameters.tickLength;
    int noteOffset = 0;
    double sum = 0.0;
    while (sum < timeOffset) {
        sum += parameters.noteLengths[noteOffset++];
    }
    
    // output file
    std::ofstream out("score.ski");
    
    int** score = parameters.allocateEmptyScore();
    fillScore(score, bestSeries);
    
    std::vector<SkiniNote> notes;
    int lastNote[3] = { -1, -1, -1 };
    for (int k = 0; k < 2; ++k) {
        for (int j = 0; j < parameters.voiceCount; ++j) {
            double time = std::max(double(timeOffset*j), double(k*parameters.tickCount*parameters.tickLength));
            for (int i = (k == 0) ? noteOffset*j : 0; i < 12; ++i) {
                int s = i - noteOffset*j;
                if (s < 0) {
                    s = 12 + s;
                }
                SkiniNote n;
                n.time = time;
                n.voice = j;
                n.note = bestSeries[s]+48;
                notes.push_back(n);
                time += parameters.noteLengths[s];
            }
        }
    }
    
    for (std::vector<SkiniNote>::iterator i = notes.begin(); i != notes.end(); ++i) {
        std::cout << i->time << " " << i->voice << " " << i->note << std::endl;
    }
    std::sort(notes.begin(), notes.end(), compareNotes);
    
    
    double currentTime = 0.0;
    for (std::vector<SkiniNote>::iterator i = notes.begin(); i != notes.end(); ++i) {
        //if (lastNote[i->voice] > 0)
        //    printNoteOff(out, i->time - currentTime, i->voice, lastNote[i->voice]);
        printNote(out, i->time - currentTime, i->voice, i->note);
        lastNote[i->voice] = i->note;
        currentTime = i->time;
    }
}

int RoundComputation::getNumberOfElements()  {
    //return 479001600;
    return 43545600;
}

std::string RoundComputation::getText(int count) {
    std::stringstream txt;
    txt << "Processing twelve tone series: " << count/1000000 << " Mio.";
    return txt.str();
}

double rating[12] = {
    2.0, 0.0, 1.0, 3.0, 3.0, 2.0, 1.0, 2.0, 3.0, 3.0, 1.0, 0.0
};

int MAX_COUNT = 479001600; // 12!
//int MAX_COUNT = 3000000;

int MAX_SUB_COUNT = 39916800;



void RoundComputation::fillSequence(int* seq, int t[12], int offset) {
    for (int i = 0; i < parameters.tickCount; ++i) {
        int tickIndex = i - offset;
        if (tickIndex < 0) {
            tickIndex = parameters.tickCount + tickIndex;
        }
        seq[i] = t[parameters.ticks[tickIndex]];
    }
}

void RoundComputation::fillScore(int** score, int t[12]) {
//    fillSequence(score[0], t, 0);
//    fillSequence(score[1], t, parameters.offset);
//    fillSequence(score[2], t, parameters.offset*2);
    for (int j = 0; j < parameters.voiceCount; ++j) {
        fillSequence(score[j], t, parameters.offset*j);
    }
}

double RoundComputation::scoreTriad(int a, int b, int c) {
    // should sound imperfect:
    int d1 = abs(a - b) % 12;
    int d2 = abs(a - c) % 12;
    int d3 = abs(b - c) % 12;
    
    return rating[d1] + rating[d2] + rating[d3];
}

double RoundComputation::score(int** score) {
    double s = 0.0;
    for (int i = 0; i < parameters.tickCount; ++i) {
        s += scoreTriad(score[0][i], score[1][i], score[2][i]);
    }
    return s;
}

void RoundComputation::processSubSeries(int nr, ComputationChunk& chunk, int baseProgress, const Thread& thread) {
    int** s = parameters.allocateEmptyScore();
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
        fillScore(s, series);
        
        double sc = score(s);
        if (sc > bestScore) {
            bestScore = sc;
            memcpy(bestSeries, series, 12*sizeof(int));
        }
        
        if (++c == REPORT_PROGRESS_COUNT) {
            // report progress:
            chunk.setProgress(baseProgress + c);
            baseProgress += c;
            c = 0;
        }
    } while (!thread.isCancelled() && std::next_permutation(series+1, series+11));
    chunk.setProgress(baseProgress + c);
}

void RoundComputation::processChunk(int threadNumber, ComputationChunk& chunk, const Thread& thread) {
    for (int i = 0; i < 12; ++i) {
        if (i % threadCount == threadNumber) {
            processSubSeries(i, chunk, chunk.getProgress(), thread);
        }
    }
}