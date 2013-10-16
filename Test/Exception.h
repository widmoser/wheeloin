//
//  Exception.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/16/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Exception__
#define __steeringwheel__Exception__

#include <iostream>
#include <string>

class Exception {
public:
    Exception(std::string message) : message(message) {}
    
    const std::string& getMessage();
    
private:
    std::string message;
};


#endif /* defined(__steeringwheel__Exception__) */
