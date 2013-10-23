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

#define sign(x) (( x >> 31 ) | ( (unsigned int)( -x ) >> 31 ))

const int BEST_N = 1000;

int gcd(int a, int b)
{
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

int fac(int n) {
    int res = 1;
    for (int i = 2; i <= n; ++i) {
        res *= i;
    }
    return res;
}

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
    
    tickWeight = new double[tickCount];
    int measureLength = 4.0 / tickLength;
    for (int i = 0; i < tickCount; ++i) {
        tickWeight[i] = double(gcd(i, measureLength)) / double(measureLength);
    }
}

Parameters::~Parameters() {
    delete ticks;
    delete tickWeight;
}

int** Parameters::allocateEmptyScore() {
    int** res = new int*[voiceCount];
    for (int i = 0; i < voiceCount; ++i) {
        res[i] = new int[tickCount];
    }
    return res;
}

const int REPORT_PROGRESS_COUNT = 100000;
const int EXPLORATION_RANGE = 11;

Series RoundComputation::fillScore(Score& score) {
    std::ifstream input("cache");
    int voiceCount;
    double offset;
    double l[12];
    
    int seqCount;
    
    input >> seqCount;
    input >> voiceCount >> offset >> l[0] >> l[1] >> l[2] >> l[3] >> l[4] >> l[5] >> l[6] >> l[7] >> l[8] >> l[9] >> l[10] >> l[11];
    Parameters parameters(voiceCount, offset, l);
    int seqIndex = rand() % seqCount;
    for (int i = 0; i < seqIndex; i++) {
        input.ignore(std::numeric_limits<unsigned int>::max(), '\n');
    }
    Series res;
    int seq[12];
    input >> res.score >> res.data[0] >> res.data[1] >> res.data[2] >> res.data[3] >> res.data[4] >> res.data[5] >> res.data[6] >> res.data[7] >> res.data[8] >> res.data[9] >> res.data[10] >> res.data[11];
    
    fillScore(score, parameters, seq);
    return res;
}

void RoundComputation::fillScore(Score& score, Parameters& parameters, int series[12]) {
    double time = 5.0;
    for (int i = 0; i < 12; ++i) {
        Note n(0, series[i]+48, time, parameters.noteLengths[i], 1.0, 1.0);
        score.addNote(n);
        time += parameters.noteLengths[i];
    }
    
    time += 8.0;
    for (int i = 0; i < 12; ++i) {
        Note n(1, series[i]+48, time, parameters.noteLengths[i], 1.0, 1.0);
        score.addNote(n);
        time += parameters.noteLengths[i];
    }
    
    for (int i = 0; i < 12; ++i) {
        Note n(1, series[i]+60, time, parameters.noteLengths[i], 1.0, 1.0);
        score.addNote(n);
        time += parameters.noteLengths[i];
    }
    
    for (int i = 0; i < 12; ++i) {
        Note n(1, series[i]+36, time, parameters.noteLengths[i], 1.0, 1.0);
        score.addNote(n);
        time += parameters.noteLengths[i];
    }
}

RoundComputation::RoundComputation(System& system, Parameters& parameters, int threads, Piece& next) : Computation(system, threads), parameters(parameters), queues(threads), result(BEST_N), next(next), chords(0) {
}

RoundComputation::~RoundComputation() {
    if (chords) {
        delete chords;
        chords = 0;
    }
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

void RoundComputation::init() {
    initializeChords();
    Computation::init();
}

int& RoundComputation::chord(int i, int j) {
    return chords[i*12 + j];
}

void RoundComputation::setChord(int ch, int i, int j) {
    int& chref = chord(i, j);
    if (chref >= 0 && chref != ch)
        std::cout << "WARNING Chord (" << i << ", " << j << ") has a conflicting assignment " << chref << " vs. " << ch << std::endl;
    chref = ch;
}

void RoundComputation::initializeChord(int ch, int i, int j) {
    setChord(ch, i, j);
    int inv = 12 - (i+j);
    if (inv < 12) {
        setChord(ch, j, inv);
        setChord(ch, inv, i);
    }
}

void RoundComputation::initializeChords() {
    chords = new int[12*12];
    std::fill_n(chords, 144, -1);
    initializeChord(PURE, 0, 0);
    initializeChord(PURE, 0, 7);
    initializeChord(FOURTH, 5, 5);
    initializeChord(FOURTH, 0, 5);
    initializeChord(MAJ, 4, 3);
    initializeChord(MAJ, 0, 4);
    initializeChord(MAJ, 0, 8);
    initializeChord(MIN, 3, 4);
    initializeChord(MIN, 0, 3);
    initializeChord(MIN, 0, 9);
    initializeChord(MAJ_7, 4, 6);
    initializeChord(MIN_7, 3, 7);
    initializeChord(DIM, 3, 3);
    initializeChord(DIM, 0, 6);
    initializeChord(PURE_7, 7, 3);
    initializeChord(PURE_7, 0, 10);
    initializeChord(DIM_7, 6, 4);
    initializeChord(AUG, 4, 4);
    initializeChord(MAJ_M7, 4, 7);
    initializeChord(MIN_M7, 3, 8);
    initializeChord(DIM_M7, 6, 5);
    initializeChord(PURE_M7, 7, 4);
    initializeChord(SOFT_CLUSTER, 0, 2);
    initializeChord(SOFT_CLUSTER, 2, 2);
    initializeChord(SOFT_CLUSTER, 2, 9);
    initializeChord(HARD_CLUSTER, 0, 1);
    initializeChord(HARD_CLUSTER, 0, 11);
    initializeChord(HARD_CLUSTER, 1, 0);
    initializeChord(HARD_CLUSTER, 1, 1);
    initializeChord(HARD_CLUSTER, 1, 5);
    initializeChord(HARD_CLUSTER, 1, 8);
    initializeChord(HARD_CLUSTER, 1, 9);
    initializeChord(HARD_CLUSTER, 1, 10);
    initializeChord(HARD_CLUSTER, 1, 11);

    // print unset chords:
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            if (chord(i, j) < 0 && i + j <= 12) {
                std::cout << "WARNING Chord (" << i << ", " << j << ") is not defined" << std::endl;
            }
        }
    }
}

void RoundComputation::computeFragmentedScore(Score& score, Parameters& parameters, int series[12]) {
    double time = 15.0;
    Note firstBass(1, rand() % 12 + 26, time, 0.0, 1.0, 1.0);
    Note& lastBass = score.addNote(firstBass);
    for (int i = 0; i < 100; ++i) {
        int fragmentLength = rand() % 6;
        int fragmentStart = rand() % 12;
        int octave = rand() % 3 + 3;
        double gap = rand() % 8 * 0.5;
        
        if (rand() % 10 > 8) {
            for (int j = fragmentStart; j < fragmentStart + fragmentLength; ++j) {
                int index = j % 12;
                Note n(2, series[index]+octave*12, time, parameters.noteLengths[index], 1.0, 1.0);
                score.addNote(n);
                time += parameters.noteLengths[index];
            }
        } else {
            lastBass.length = time - lastBass.start;
            Note n(1, rand() % 12 + 26, time, 0.0, 1.0, 1.0);
            lastBass = score.addNote(n);
        }
        time += gap;
    }
}

Series RoundComputation::popMin() {
    typedef std::vector<std::priority_queue<Series> >::iterator iter;
    double min = std::numeric_limits<double>::max();
    iter minIter;
    for (iter i = queues.begin(); i != queues.end(); ++i) {
        if (!i->empty() && i->top().score < min) {
            min = i->top().score;
            minIter = i;
        }
    }
    Series res = minIter->top();
    minIter->pop();
    return res;
}

void RoundComputation::merge() {
    typedef std::vector<std::priority_queue<Series> >::iterator iter;
    unsigned long solutionCount = queues.size()*BEST_N;
    
    while (solutionCount > BEST_N) {
        popMin();
        solutionCount--;
    }
    
    for (int i = 0; i < result.size(); ++i) {
        result[i] = popMin();
    }
}

void RoundComputation::finalize() {
    Computation::finalize();
    
    merge();
    
    for (std::vector<Series>::iterator i = result.begin(); i != result.end(); ++i) {
        std::cout << "Score: " << i->score << "/" << parameters.tickCount << std::endl << "Series: ";
        for (int j = 0; j < 12; ++j) {
            std::cout << i->data[j] << " ";
        }
        std::cout << std::endl;
    }
    
    double timeOffset = parameters.offset*parameters.tickLength;
    int noteOffset = 0;
    double sum = 0.0;
    while (sum < timeOffset) {
        sum += parameters.noteLengths[noteOffset++];
    }
    
    int* resultSequence = result.back().data;
    fillScore(bestScore, parameters, resultSequence);
    
    // output file
    std::ofstream out("score.ski");
    
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
                n.note = resultSequence[s]+48;
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
    
    delete chords;
    
    std::ofstream cache("cache");
    cache << result.size() << std::endl;
    cache << parameters.voiceCount << " " << parameters.offset;
    for (int i = 0; i < 12; ++i) {
        cache << " " << parameters.noteLengths[i];
    }
    for (std::vector<Series>::iterator i = result.begin(); i != result.end(); ++i) {
        cache << std::endl << i->score;
        for (int j = 0; j < 12; ++j) {
            cache << " " << i->data[j];
        }
    }
    
    next.setScore(bestScore);
}

Score& RoundComputation::getScore() {
    return bestScore;
}

int RoundComputation::getNumberOfElements()  {
    return 12*fac(EXPLORATION_RANGE-1);
}

std::string RoundComputation::getText(int count) {
    std::stringstream txt;
    txt << "Processing twelve tone series: " << count/1000000 << " Mio.";
    return txt.str();
}

double rating[12] = {
    2.0, 0.0, 1.0, 3.0, 3.0, 2.0, 1.0, 2.0, 3.0, 3.0, 1.0, 0.0
};

double parallel[12] = {
    0.7, 0.6, 0.4, 0.0, 0.0, 0.4, 0.6, 0.3, 0.0, 0.0, 0.6, 0.7
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

int RoundComputation::getChord(int a, int b, int c, int& d1, int& d2) {
    int n0, n1, n2;
    if (a < b) {
        if (b < c) {
            n0 = a;
            n1 = b;
            n2 = c;
        } else {
            if (a < c) {
                n0 = a;
                n2 = b;
                n1 = c;
            } else {
                n1 = a;
                n2 = b;
                n0 = c;
            }
        }
    } else {
        if (a < c) {
            n1 = a;
            n0 = b;
            n2 = c;
        } else {
            if (a < b) {
                n2 = a;
                n0 = b;
                n1 = c;
            } else {
                n2 = a;
                n1 = b;
                n0 = c;
            }
        }
    }
    
    d1 = n1 - n0;
    d2 = n2 - n1;
    
    return chord(d1, d2);
}

double RoundComputation::score(int** score) {
    double s = 0.0;
    int lastChord = 0;
    int lastD1 = -1;
    int lastD2 = -1;
    int lastA, lastB, lastC;
    int ch, d1, d2;
    for (int i = 0; i < parameters.tickCount; ++i) {
        int a = score[0][i];
        int b = score[1][i];
        int c = score[2][i];
        int ch = getChord(a, b, c, d1, d2);
        if (ch != lastChord) {
            s += (1.0 / abs(ch - lastChord))*2.0;
        } else {
            s += 1.0 - parameters.tickWeight[i];
        }
        if (d1 == lastD1) {
            s -= parallel[d1];
        }
        if (d2 == lastD2) {
            s -= parallel[d2];
        }
        
        int sA = sign(a - lastA);
        int sB = sign(b - lastB);
        int sC = sign(c - lastC);
        if (i > 0 && sA == sB && sB == sC) {
            s -= 0.1;
        }
        
        lastChord = ch;
        lastD1 = d1;
        lastD2 = d2;
        lastA = a;
        lastB = b;
        lastC = c;
    }
    return s;
}

void RoundComputation::processSubSeries(int nr, ComputationChunk& chunk, int baseProgress, const Thread& thread, std::priority_queue<Series>& bestSeries) {
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
        if (bestSeries.size() < BEST_N || sc > bestSeries.top().score) {
            Series s;
            s.score = sc;
            memcpy(s.data, series, 12*sizeof(int));
            bestSeries.push(s);
            if (bestSeries.size() > BEST_N) {
                bestSeries.pop();
            }
        }
        
        if (++c == REPORT_PROGRESS_COUNT) {
            // report progress:
            chunk.setProgress(baseProgress + c);
            baseProgress += c;
            c = 0;
        }
    } while (!thread.isCancelled() && std::next_permutation(series+1, series+EXPLORATION_RANGE));
    chunk.setProgress(baseProgress + c);
    
    delete s;
}

void RoundComputation::processChunk(int threadNumber, ComputationChunk& chunk, const Thread& thread) {
    for (int i = 0; i < 12; ++i) {
        if (i % threadCount == threadNumber) {
            processSubSeries(i, chunk, chunk.getProgress(), thread, queues[threadNumber]);
        }
    }
}