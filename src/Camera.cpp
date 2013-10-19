//
//  Camera.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/19/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Camera.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

void Camera::update() {
    glMatrixMode(GL_MODELVIEW); //select the matrix
    glLoadIdentity(); //clear ...
    glm::vec3 lookAt = position + direction;
    lookAt.y = 0;
    gluLookAt(position.x, position.y, position.z, lookAt.x, lookAt.y, lookAt.z, 0, 1, 0);
}