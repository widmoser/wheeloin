//
//  ScoreDisplay.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "ScoreDisplay.h"
#include <sstream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

std::string names[] = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "H"
};


std::string getNoteName(int note) {
    if (note > 4) {
        return names[(note-4) % 12];
    } else {
        return "";
    }
}

ScoreDisplay::ScoreDisplay(Wheeloin& instrument, Renderer& renderer) : instrument(instrument), renderer(renderer), position(0.0f), gridWidth(0.3f), gridLength(1.0f) {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5f);
}

float ScoreDisplay::getAspect() {
    return float(renderer.getDisplayWidth()) / float(renderer.getDisplayHeight());
}

void ScoreDisplay::setProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, getAspect(), 0.1, 1000.0);
}

void ScoreDisplay::setCamera(float note, float time) {
    glMatrixMode(GL_MODELVIEW); //select the matrix
    glLoadIdentity(); //clear ...
    gluLookAt(note , 1, -time, note, 0, -time-0.9, 0, 1, 0);
}

void ScoreDisplay::setView() {
    setProjection();
    setCamera(float(instrument.getInputScaleNote())*gridWidth, position);
}

void ScoreDisplay::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ScoreDisplay::drawGrid() {
    WheeloinConfiguration& c = instrument.getConfiguration();
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
    for (int i = c.minNote; i <= c.maxNote+1; ++i) {
        float x = (i-0.5f)*gridWidth;
        glVertex3f(x, 0.0f, 100.0f);
        glVertex3f(x ,0.0f, -position-500.0f);
    }
    for (int i = 0; i > -500; --i) {
        float z = i*gridLength;
        glVertex3f(-100,0,z);
        glVertex3f(100,0,z);
    }
    glEnd();
}

void ScoreDisplay::drawNotes() {
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(-0.05f, 0.0f, -50.0f);
    glVertex3f(0.05f, 0.0f, -50.0f);
    glVertex3f(0.05f, 0.0f, -100.0f);
    glVertex3f(-0.05f, 0.0f, -100.0f);
    glEnd();
}

void ScoreDisplay::drawCursor() {
    glColor3f(0.7f, 0.8f, 1.0f);
    float radius = 0.5f*gridWidth*float(instrument.getVolume());
    float centerX = float(instrument.getInputScaleNote())*gridWidth;
    float centerZ = -  position - 0.1f;
    glBegin(GL_LINE_LOOP);
    for (int i=0; i < 90; ++i)
    {
        float degInRad = i*4*0.0174532925f;
        glVertex3f(cosf(degInRad)*radius+centerX,0,sinf(degInRad)*radius + centerZ);
    }
    glEnd();
    
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex3f(centerX, 0, centerZ);
    glEnd();
}

void ScoreDisplay::draw() {
    clear();
    
    setView();
    drawGrid();
    drawNotes();
    drawCursor();
    
    drawTextOverlay();
    
    position += 0.01f;
}

void ScoreDisplay::drawTextOverlay() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, renderer.getDisplayWidth(), renderer.getDisplayHeight(), 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    
    std::stringstream text;
    std::stringstream text2;
    
    text << "Group: " << instrument.getActiveVoice();
    text2 << instrument.getVolume() << " " << instrument.getInputScaleNote() << " " << getNoteName(instrument.getInputNote());
    renderer.drawText(50, 50, text.str());
    renderer.drawText(50, 100, text2.str());
}