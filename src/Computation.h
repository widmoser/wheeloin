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
    Computation(System& system, int threadCount);
    virtual ~Computation();
    void init();
    virtual bool frame();
    void onCancel();
    
    
    virtual void processChunk(int threadNumber, ComputationChunk& runnable, const Thread& thread) = 0;
protected:
    virtual int getNumberOfElements() = 0;
    virtual std::string getText(int count) = 0;
    
    int threadCount;
private:
    void draw(int elements);
    Renderer& renderer;
    std::vector<Thread*> threads;
    std::vector<ComputationChunk*> chunks;
};

class ComputationChunk : public Runnable {
public:
    ComputationChunk(Computation& computation, int threadNumber) : computation(computation), threadNumber(threadNumber), progress(0) {}
    void run(const Thread& thread) {
        computation.processChunk(threadNumber, *this, thread);
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
