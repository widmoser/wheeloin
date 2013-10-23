//
//  Pause.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Pause.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


Pause::Pause(System& system) : Phase(system), system(system) {
    
}

void Pause::init() {
    Phase::init();
    system.getRenderer().setTextFont("OpenSans-Regular.ttf", 60);
    system.getRenderer().setTextColor(255, 255, 255);
}

bool Pause::frame() {
    float w = system.getRenderer().getDisplayWidth();
    float h = system.getRenderer().getDisplayHeight();
    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w, h, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    
    system.getRenderer().drawText(w*0.5f, h*0.5f, "Press SPACE to continue...");
    
    if (system.getKeyboard().isButtonDown(75)) {
        // space:
        return false;
    } else {
        return true;
    }
}