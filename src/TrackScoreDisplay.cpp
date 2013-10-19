//
//  TrackScoreDisplay.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/19/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "TrackScoreDisplay.h"
#include <sstream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <glm/gtx/rotate_vector.hpp>
#include <Scale.h>

TrackScoreDisplay::TrackScoreDisplay(Wheeloin& instrument, System& system, Score& score) : instrument(instrument), system(system), renderer(system.getRenderer()), score(score), position(0.0f), gridWidth(0.3f), gridLength(1.0f) {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5f);
    
    camera.position = glm::vec3(0, 1.2, 0);
    camera.direction = glm::vec3(0, 0, -1);
}

float TrackScoreDisplay::getAspect() {
    return float(renderer.getDisplayWidth()) / float(renderer.getDisplayHeight());
}

void TrackScoreDisplay::setProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, getAspect(), 0.1, 1000.0);
}

void TrackScoreDisplay::setCamera(float note, float time, float delta) {
    float angle = -system.getJoystick().getAxis(0)*0.06f;
    
    camera.direction = glm::rotate(camera.direction, angle, glm::vec3(0, 1, 0));
    
    camera.position += camera.direction*delta;
    //camera.direction = glm::vec3(0, 0, -1);
    camera.update();
}

void TrackScoreDisplay::setView(float delta) {
    setProjection();
    setCamera(float(instrument.getInputScaleNote())*gridWidth, position, delta);
}

void TrackScoreDisplay::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void TrackScoreDisplay::drawGrid() {
    WheeloinConfiguration& c = instrument.getConfiguration();
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
    for (int i = c.minNote; i <= c.maxNote+1; ++i) {
        float x = (i-0.5f)*gridWidth;
        glVertex3f(x, 0.0f, 100.0f);
        glVertex3f(x ,0.0f, -500.0f);
    }
    for (int i = 0; i > -500; --i) {
        float z = i*gridLength;
        glVertex3f(-100,0,z);
        glVertex3f(100,0,z);
    }
    glEnd();
}

void TrackScoreDisplay::drawNotes() {
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    glm::vec3 dir = glm::vec3(0, 0, -0.01f);
    glm::vec3 pos = glm::vec3(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(pos.x, pos.y, pos.z);
    for (int i = 0; i < 300; ++i) {
        pos+=dir;
        glVertex3f(pos.x, pos.y, pos.z);
    }
    for (int i = 0; i < 1000; ++i) {
        dir = glm::rotate(dir, getAngle(40), glm::vec3(0, 1, 0));
        pos+=dir;
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
}

void TrackScoreDisplay::drawCursor() {
    glColor3f(0.7f, 0.8f, 1.0f);
    float radius = 0.5f*gridWidth*float(instrument.getVolume());
    glm::vec3 center = camera.position + camera.direction * 0.1f;
    glBegin(GL_LINE_LOOP);
    for (int i=0; i < 90; ++i)
    {
        float degInRad = i*4*0.0174532925f;
        glVertex3f(cosf(degInRad)*radius+center.x,0,sinf(degInRad)*radius + center.z);
    }
    glEnd();
    
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex3f(center.x, 0, center.z);
    glEnd();
}

void TrackScoreDisplay::draw() {
    clear();
    
    setView(float(system.getTime()) - position);
    drawGrid();
    drawNotes();
    drawCursor();
    
    drawTextOverlay(float(system.getTime()) - position);
    position = float(system.getTime());
}

void TrackScoreDisplay::drawTextOverlay(float delta) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, renderer.getDisplayWidth(), renderer.getDisplayHeight(), 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    
    std::stringstream text;
    std::stringstream text2;
    std::stringstream display;
    
    text << "Group: " << instrument.getActiveVoice() << " " << system.getTime() << " FPS: " << 1.0f / delta;
    text2 << instrument.getVolume() << " " << instrument.getInputScaleNote() << " " << getNoteName(instrument.getInputNote());
    display << "(" << renderer.getDisplayWidth() << ", " << renderer.getDisplayHeight() << ")";
    renderer.drawText(50, 50, text.str());
    renderer.drawText(50, 100, text2.str());
    renderer.drawText(50, 150, display.str());
}

float TrackScoreDisplay::getAngle(int note) {
    WheeloinConfiguration& c = instrument.getConfiguration();
    int relNote = note - c.minNote;
    float range = c.maxNote - c.minNote;
    float invFactor = 2.0f / range;
    float raw = -relNote*invFactor + 1.0f;
    return raw*0.06f;
}