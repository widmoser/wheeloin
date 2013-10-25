//
//  Phase.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Phase__
#define __steeringwheel__Phase__

#include <iostream>
#include <engine/System.h>

class Phase {
public:
    Phase(System& system);
    
    virtual void init();
    virtual void finalize() {}
    virtual void onCancel() {}
    virtual bool frame() = 0;
    
    double getTime();
    
    System& getSystem();
private:
    System& system;
    double startTime;
};

#endif /* defined(__steeringwheel__Phase__) */
