//
//  Thread.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/22/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Thread__
#define __steeringwheel__Thread__

#include <iostream>

class Thread;

class Runnable {
public:
    virtual void run(const Thread& thread) = 0;
};

class Thread {
public:
    Thread(Runnable& runnable) : runnable(runnable) {}
    virtual ~Thread() {};
    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual void join() = 0;
    virtual bool isCancelled() const = 0;
protected:
    Runnable& runnable;
};

#endif /* defined(__steeringwheel__Thread__) */
