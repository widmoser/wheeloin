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
#include <Scale.h>

float voiceColors[3][3] = {
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f}
};

ScoreDisplay::ScoreDisplay(Wheeloin& instrument, System& system, Score& score) : instrument(instrument), system(system), renderer(system.getRenderer()), score(score), position(0.0f), gridWidth(0.3f), gridLength(1.0f) {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5f);
}

void ScoreDisplay::init() {
    renderer.setTextFont("OpenSans-Regular.ttf", 20);
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
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5f);
}

void ScoreDisplay::processInput() {
    if (system.getJoystick().isButtonDown(0) || system.getJoystick().isButtonDown(1)) {
        Note& n = score.nextNote();
        int note = instrument.getInputNote();
        if (system.getJoystick().isButtonDown(0)) {
            note += 1;
        }
        if (note == n.value && instrument.getActiveVoice() == n.voice) {
            if (!n.activated) {
                n.activated = true;
                score.popNote();
            }
        }
    }
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
    const std::vector<Note> notes = score.getNotes();
    glBegin(GL_QUADS);
    for (std::vector<Note>::const_iterator i = notes.begin(); i != notes.end(); ++i) {
        Note n = *i;
        glColor4f(voiceColors[n.voice][0], voiceColors[n.voice][1], voiceColors[n.voice][2], n.activated ? 1.0f : 0.35f);
        bool accidental = false;
        int scaleNote = instrument.getConfiguration().scale.getScaleNote(n.value, accidental);
        glVertex3f((scaleNote - float(n.startVolume)*0.5f)*gridWidth, 0.0f, float(-n.start));
        glVertex3f((scaleNote + float(n.startVolume)*0.5f)*gridWidth, 0.0f, float(-n.start));
        glVertex3f((scaleNote + float(n.endVolume)*0.5f)*gridWidth, 0.0f, float(-n.start - n.length));
        glVertex3f((scaleNote - float(n.endVolume)*0.5f)*gridWidth, 0.0f, float(-n.start - n.length));
    }
    glEnd();
}

void ScoreDisplay::drawCursor() {
    Note& n = score.nextNote();
    int note = int(round(instrument.getInputScaleNote()));
    bool accidental = false;
    float dist = float(system.getTime() - score.nextNote().start);
    float r = voiceColors[instrument.getActiveVoice()][0];
    float g = voiceColors[instrument.getActiveVoice()][1];
    float b = voiceColors[instrument.getActiveVoice()][2];
    int nextScaleNote = instrument.getConfiguration().scale.getScaleNote(n.value, accidental);
    if (fabsf(dist) < 0.1f && note == nextScaleNote && instrument.getActiveVoice() == n.voice) {
        float relDist = 1.0f - fabsf(dist)*10.0f;
        r = (1.0f - r)*relDist + r;
        g = (1.0f - g)*relDist + g;
        b = (1.0f - b)*relDist + b;
    }
    glColor3f(r, g, b);
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
    
    if (accidental) {
        glBegin(GL_LINES);
        glVertex3f(centerX - radius, 0, centerZ - radius);
        glVertex3f(centerX + radius, 0, centerZ + radius);
        glVertex3f(centerX + radius, 0, centerZ - radius);
        glVertex3f(centerX - radius, 0, centerZ + radius);
        glEnd();
    }
}

void ScoreDisplay::draw() {
    processInput();
    clear();
    
    setView();
    drawGrid();
    drawNotes();
    drawCursor();
    
    drawTextOverlay(float(system.getTime()) - position);
    position = float(system.getTime());
}

void ScoreDisplay::drawTextOverlay(float delta) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, renderer.getDisplayWidth(), renderer.getDisplayHeight(), 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    
    std::stringstream text;
    std::stringstream text2;
    
    text << "Group: " << instrument.getActiveVoice() << " " << system.getTime() << " FPS: " << 1.0f / delta;
    text2 << instrument.getVolume() << " " << instrument.getInputScaleNote() << " " << getNoteName(instrument.getInputNote());
    renderer.drawText(50, 50, text.str());
    renderer.drawText(50, 100, text2.str());
}