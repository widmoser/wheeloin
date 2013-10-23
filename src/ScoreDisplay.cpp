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
#include <Piece.h>

float voiceColors[3][3] = {
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f}
};

ScoreDisplay::ScoreDisplay(Piece& piece, Score& score) : piece(piece), system(piece.getSystem()), instrument(piece.getInstrument()), renderer(piece.getSystem().getRenderer()), score(score), position(0.0f), gridWidth(0.3f), gridLength(1.0f) {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5f);
}

void ScoreDisplay::init() {
    renderer.setTextFont("OpenSans-Regular.ttf", 40);
}

void ScoreDisplay::setScore(Score& score) {
    this->score = score;
}

Score& ScoreDisplay::getScore() {
    return score;
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
    
    for (std::vector<Note>::const_iterator i = notes.begin(); i != notes.end(); ++i) {
        Note n = *i;
        
        bool accidental = false;
        int scaleNote = instrument.getConfiguration().scale.getScaleNote(n.value, accidental);
        
        float alpha = n.activated ? 1.0f : 0.35f;
        float w = float(n.startVolume)*0.5f;
        float l = (scaleNote - w)*gridWidth;
        float r = (scaleNote + w)*gridWidth;
        float s = float(-n.start);
        float e = float(-n.start - n.length);
        
        if (!accidental) {
            glBegin(GL_TRIANGLE_STRIP);
            glColor4f(1.0f, 1.0f, 1.0f, alpha);
            glVertex3f(l, 0.0f, s);
            glVertex3f(r, 0.0f, s);
            glVertex3f(l, 0.0f, s-0.1f);
            glVertex3f(r, 0.0f, s-0.1f);
            glEnd();
        } else {
            glBegin(GL_TRIANGLES);
            glColor4f(1.0f, 1.0f, 1.0f, alpha);
            glVertex3f(l, 0.0f, s);
            glVertex3f(r, 0.0f, s);
            glVertex3f((l+r)*0.5f, 0.0f, s-0.1f);
            glEnd();
        }
        
        glBegin(GL_TRIANGLE_STRIP);
//        glColor4f(1.0f, 1.0f, 1.0f, 0.35f);
//        glVertex3f(l, 0.3f, s);
//        glVertex3f(r, 0.3f, s);
        glColor4f(voiceColors[n.voice][0], voiceColors[n.voice][1], voiceColors[n.voice][2], alpha);
        glVertex3f(l, 0.0f, s-0.1f);
        glVertex3f(r, 0.0f, s-0.1f);
        glVertex3f(l, 0.0f, e);
        glVertex3f(r, 0.0f, e);
        glEnd();
    }
}

void ScoreDisplay::drawCursor() {    
    float r = voiceColors[instrument.getActiveVoice()][0];
    float g = voiceColors[instrument.getActiveVoice()][1];
    float b = voiceColors[instrument.getActiveVoice()][2];
    bool accidental = false;
    if (score.hasNextNote()) {
        Note& n = score.nextNote();
        int note = int(round(instrument.getInputScaleNote()));
        float dist = float(system.getTime() - score.nextNote().start);
        int nextScaleNote = instrument.getConfiguration().scale.getScaleNote(n.value, accidental);
        if (fabsf(dist) < 0.1f && note == nextScaleNote && instrument.getActiveVoice() == n.voice) {
            float relDist = 1.0f - fabsf(dist)*10.0f;
            r = (1.0f - r)*relDist + r;
            g = (1.0f - g)*relDist + g;
            b = (1.0f - b)*relDist + b;
        }
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
    
    drawTextOverlay(float(piece.getTime()) - position);
    position = float(piece.getTime());
    
    drawOverlay();
}


void ScoreDisplay::drawOverlay() {
    if (piece.getTime() > score.getLength()) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 1.0, 1.0, 0.0, -1.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_CULL_FACE);
        
        float diff = float(piece.getTime() - score.getLength());
        float alpha = diff*0.2f;
        glColor4f(0.0f, 0.0f, 0.0f, alpha);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glEnd();
    }
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
    
//    text << "Group: " << instrument.getActiveVoice() << " " << system.getTime() << " FPS: " << 1.0f / delta;
//    text2 << instrument.getVolume() << " " << instrument.getInputScaleNote() << " " << getNoteName(instrument.getInputNote());
//    renderer.setTextColor(255, 255, 255);
//    renderer.drawText(50, 50, text.str());
//    renderer.drawText(50, 100, text2.str());
    
    double t = piece.getTime();
    if (t < 1.0) {
        renderer.setTextColor(int(t*255), int(t*255), int(t*255));
    } else if (t >= 2.0 && t < 3.0) {
        renderer.setTextColor((3.0 - t)*255, (3.0 - t)*255, (3.0 - t)*255);
    }
    
    if (t < 3.0) {
        renderer.drawText(renderer.getDisplayWidth()*0.5f, renderer.getDisplayHeight()*0.5f, piece.getTitle());
    }
}