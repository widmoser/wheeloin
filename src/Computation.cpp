//
//  Computation.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/20/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Computation.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


#include <engine/allegro/AllegroThread.h>

Computation::Computation(Renderer& renderer, int threadCount) : renderer(renderer), threadCount(threadCount) {
    renderer.setTextFont("OpenSans-Regular.ttf", 60);
    for (int i = 0; i < threadCount; ++i) {
        ComputationChunk* c = new ComputationChunk(*this, i);
        threads.push_back(new AllegroThread(*c));
        chunks.push_back(c);
    }
}

Computation::~Computation() {
    for (std::vector<Thread*>::iterator i = threads.begin(); i != threads.end(); ++i) {
        delete *i;
    }
    for (std::vector<ComputationChunk*>::iterator i = chunks.begin(); i != chunks.end(); ++i) {
        delete *i;
    }
}

void Computation::init() {
    renderer.setTextFont("OpenSans-Regular.ttf", 60);
    for (std::vector<Thread*>::iterator i = threads.begin(); i != threads.end(); ++i) {
        (*i)->start();
    }
}

bool Computation::frame() {
    int elements = 0;
    for (std::vector<ComputationChunk*>::iterator i = chunks.begin(); i != chunks.end(); ++i) {
        elements += (*i)->getProgress();
    }
    draw(elements);
    return elements < getNumberOfElements();
}

void Computation::draw(int elements) {
    double progress = double(elements) / double(getNumberOfElements());
    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1, 1, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    
    
    
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.7f, 1.0f);
    glVertex3f(0.2f, 0.45f, 0.0f);
    glVertex3f(0.2f + float(progress)*0.6f, 0.45f, 0.0f);
    glVertex3f(0.2f + float(progress)*0.6f, 0.55f, 0.0f);
    glVertex3f(0.2f, 0.55f, 0.0f);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0.2f, 0.45f, 0.0f);
    glVertex3f(0.8f, 0.45f, 0.0f);
    glVertex3f(0.8f, 0.55f, 0.0f);
    glVertex3f(0.2f, 0.55f, 0.0f);
    glEnd();
    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, renderer.getDisplayWidth(), renderer.getDisplayHeight(), 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    
    renderer.drawText(renderer.getDisplayWidth()*0.5f, 300, text);
}

void Computation::setText(const std::string& str) {
    text = str;
}