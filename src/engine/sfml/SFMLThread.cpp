//
//  SFMLThread.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "SFMLThread.h"

SFMLThread::SFMLThread(Runnable& runnable) : Thread(runnable), internal(&SFMLThread::run, this), cancelled(false) {
}

void SFMLThread::start() {
    internal.launch();
}

void SFMLThread::cancel() {
    cancelled = true;
}

void SFMLThread::join() {
    internal.wait();
}

bool SFMLThread::isCancelled() const {
    return cancelled;
}

void SFMLThread::run() {
    runnable.run(*this);
}