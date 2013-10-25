//
//  Camera.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/19/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__Camera__
#define __steeringwheel__Camera__

#include <iostream>
#include <glm/glm.hpp>

class Camera {
public:
    void update();

    glm::vec3 position;
    glm::vec3 direction;
};

#endif /* defined(__steeringwheel__Camera__) */
