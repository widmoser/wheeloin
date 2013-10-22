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

class Phase {
public:
    virtual void init() {}
    virtual void finalize() {}
    virtual bool frame() = 0;
};

#endif /* defined(__steeringwheel__Phase__) */
