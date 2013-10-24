//
//  SFMLThread.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__SFMLThread__
#define __steeringwheel__SFMLThread__

#include <iostream>
#include <engine/Thread.h>
#include <SFML/System/Thread.hpp>

class SFMLThread : public Thread {
public:
    SFMLThread(Runnable& runnable);
    
    virtual void start();
    virtual void cancel();
    virtual void join();
    virtual bool isCancelled() const;
private:
    void run();
    
    bool cancelled;
    sf::Thread internal;
};

#endif /* defined(__steeringwheel__SFMLThread__) */
