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
    textFont = 0;
    al_init_font_addon();
    al_init_ttf_addon();
    
    int flags = ALLEGRO_OPENGL | ALLEGRO_RESIZABLE;
    if (fullScreen) {
        flags |= ALLEGRO_FULLSCREEN_WINDOW;
    }
    
    al_set_new_display_flags(flags);
    display = al_create_display(width, height);
    if(!display){
        throw Exception("Failed to create display!");
    }
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
    if (textFont)
        al_destroy_font(textFont);
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    //al_destroy_display(display);
}

void AllegroRenderer::setTextColor(int r, int g, int b) {
    textColor = al_map_rgb(r, g, b);
}

void AllegroRenderer::setTextFont(const std::string& file, int size) {
    if (textFont) {
        al_destroy_font(textFont);
    }
    textFont = al_load_font(file.c_str(), size, 0);
}

void AllegroRenderer::drawText(float x, float y, const std::string& text) {
    al_draw_text(textFont, textColor, x, y, ALLEGRO_ALIGN_CENTRE, text.c_str());
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