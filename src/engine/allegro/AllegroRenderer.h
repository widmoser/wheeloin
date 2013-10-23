//
//  AllegroRenderer.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__AllegroRenderer__
#define __steeringwheel__AllegroRenderer__

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <engine/Renderer.h>

class AllegroRenderer : public Renderer {
public:
    AllegroRenderer(int width, int height, bool fullScreen = false);
    AllegroRenderer();
    ~AllegroRenderer();
    
    virtual void setTextColor(int r, int g, int b);
    virtual void drawText(float x, float y, const std::string& text);
    virtual void updateDisplay();
    
    virtual int getDisplayWidth();
    virtual int getDisplayHeight();
    
private:
    void init(int width, int height, bool fullScreen = false);
    
    ALLEGRO_DISPLAY* display;
    ALLEGRO_COLOR textColor;
    ALLEGRO_FONT* textFont;
};

#endif /* defined(__steeringwheel__AllegroRenderer__) */
