//
//  AllegroRenderer.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/18/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "AllegroRenderer.h"
#include <Exception.h>

void AllegroRenderer::init(int width, int height, bool fullScreen) {
    int flags = ALLEGRO_OPENGL | ALLEGRO_RESIZABLE;
    if (fullScreen) {
        flags |= ALLEGRO_FULLSCREEN_WINDOW;
    }
    
    al_set_new_display_flags(flags);
    display = al_create_display(width, height);
    if(!display){
        throw Exception("Failed to create display!");
    }
    
    textFont = al_load_font("OpenSans-Regular.ttf", 60, 0);
}

AllegroRenderer::AllegroRenderer(int width, int height, bool fullScreen) {
    init(width, height, fullScreen);
}

AllegroRenderer::AllegroRenderer() {
    ALLEGRO_DISPLAY_MODE disp_data;
    al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
    init(disp_data.width, disp_data.height, true);
}

AllegroRenderer::~AllegroRenderer() {
    al_destroy_font(textFont);
    al_destroy_display(display);
}

void AllegroRenderer::setTextColor(int r, int g, int b) {
    textColor = al_map_rgb(r, g, b);
}

void AllegroRenderer::drawText(float x, float y, const std::string& text, int size) {
    if (textFont) {
        al_draw_text(textFont, textColor, x, y, ALLEGRO_ALIGN_CENTRE, text.c_str());
    }
}

void AllegroRenderer::updateDisplay() {
    al_flip_display();
}

int AllegroRenderer::getDisplayWidth() {
    return al_get_display_width(display);
}

int AllegroRenderer::getDisplayHeight() {
    return al_get_display_height(display);
}