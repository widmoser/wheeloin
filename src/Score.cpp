//
//  Score.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "Score.h"
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

Score::Score(Wheeloin& instrument, Renderer& renderer) : instrument(instrument), renderer(renderer), position(0.0) {
}

void Score::setupOpenGlMatrices(float note, float time) {
    glMatrixMode(GL_PROJECTION); //Open the projection matrix as current
    glLoadIdentity(); //clear its contents to an identity matrix (which doesn't change anything when applied)
    gluPerspective(60, 1.0, 0.1, 1000.0);
    //this function is from the very useful glu library .
    //It configures the projection matrix to the desired parameters
    
    glMatrixMode(GL_MODELVIEW); //select the matrix
    glLoadIdentity(); //clear ...
    gluLookAt ( note , 1, -time, note, 0, -time-1, 0, 1, 0 );
}

void Score::draw() {
    WheeloinConfiguration& c = instrument.getConfiguration();
    double middle = 0.5*(c.maxNote - c.minNote) + c.minNote;
    
    //al_clear_to_color(al_map_rgb(0,0,0));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    setupOpenGlMatrices(float((instrument.getInputScaleNote()-middle)*0.1f), float(position));
    position += 0.01f;
    glBegin(GL_LINES); //lookup for more options but GL_LINES tels GL that you'll be supplying a series of lines (via points)
    
    glColor3f(0.2f, 0.2f, 0.2f);
    
    for (int i = -50; i < 50; ++i) {
        glVertex3f(i*0.1f-0.05f,0.0f,100.0f);
        glVertex3f(i*0.1f-0.05f,0.0f,-position-100.0f);
    }
    
    for (int i = -500; i < 500; ++i) {
        glVertex3f(-100,0,i);
        glVertex3f(100,0,i);
    }
    
    glEnd(); //this will draw a single line watched from the camera that we just set up
    
    glBegin(GL_QUADS);
    
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
    
    glVertex3f(-0.05f, 0.0f, -50.0f);
    glVertex3f(0.05f, 0.0f, -50.0f);
    glVertex3f(0.05f, 0.0f, -100.0f);
    glVertex3f(-0.05f, 0.0f, -100.0f);
    
    glEnd();
    
    
    glFlush();
    
    
}

void Score::drawTextOverlay() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1440, 900, 0.0, -1.0, 10.0);
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