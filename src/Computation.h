//
//  Computation.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Computation__
#define __steeringwheel__Computation__

#include <iostream>
#include <vector>
#include <engine/Phase.h>
#include <engine/Renderer.h>
#include <engine/Thread.h>

class ComputationChunk;

class Computation : public Phase {
public:
    Computation(Renderer& renderer, int threadCount);
    virtual ~Computation();
    void init();
    virtual bool frame();
    
    
    virtual void processChunk(int thread, ComputationChunk& runnable) = 0;
protected:
    void setText(const std::string& str);
    virtual int getNumberOfElements() = 0;
    
    int threadCount;
private:
    void draw(int elements);
    std::string text;
    Renderer& renderer;
    std::vector<Thread*> threads;
    std::vector<ComputationChunk*> chunks;
};

class ComputationChunk : public Runnable {
public:
    ComputationChunk(Computation& computation, int threadNumber) : computation(computation), threadNumber(threadNumber), progress(0) {}
    void run(const Thread& thread) {
        computation.processChunk(threadNumber, *this);
    }
    int getProgress() {
        return progress;
    }
    void setProgress(int value) {
        progress = value;
    }
private:
    Computation& computation;
    int threadNumber;
    int progress;
};

#endif /* defined(__steeringwheel__Computation__) */
