//
//  Scale.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Scale__
#define __steeringwheel__Scale__

#include <iostream>
#include <vector>

class Scale {
public:
    Scale(int start = 0);
    int getNote(double scaleNote) const;
    
    Scale& operator|(int arg);
    
private:
    std::vector<int> data;
};

class Scales {
public:
    static Scale _0;
    static Scale MAJOR;
};

#endif /* defined(__steeringwheel__Scale__) */
