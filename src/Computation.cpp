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
#include <sstream>
#include <iomanip>

#include <engine/allegro/AllegroThread.h>

Computation::Computation(System& system, int threadCount) : Phase(system), renderer(system.getRenderer()), threadCount(threadCount) {
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
    Phase::init();
    for (std::vector<Thread*>::iterator i = threads.begin(); i != threads.end(); ++i) {
        (*i)->start();
    }
}


void Computation::finalize() {
    Phase::finalize();
    for (std::vector<Thread*>::iterator i = threads.begin(); i != threads.end(); ++i) {
        (*i)->cancel();
        (*i)->join();
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

void Computation::onCancel() {
    Phase::onCancel();
    for (std::vector<Thread*>::iterator i = threads.begin(); i != threads.end(); ++i) {
        (*i)->cancel();
        (*i)->join();
    }
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

    renderer.drawText(renderer.getDisplayWidth()*0.5f, 300, getText(elements));
    
    std::stringstream clock;
    double time = getTime();
    int minutes = int(time / 60.0);
    int seconds = time - minutes*60;
    clock << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    renderer.drawText(renderer.getDisplayWidth()*0.5f, renderer.getDisplayHeight()*0.5f + 100, clock.str());
}