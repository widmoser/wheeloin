//
//  SFMLRenderer.cpp
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#include "SFMLRenderer.h"

SFMLRenderer::SFMLRenderer(int width, int height, bool fullScreen) : window(sf::VideoMode(width, height), "Wheeloin v0.1", fullScreen ? sf::Style::Fullscreen : sf::Style::Default) {
    init();
}

SFMLRenderer::SFMLRenderer() : window(sf::VideoMode::getFullscreenModes().back(), "Wheeloin v0.1", sf::Style::Fullscreen) {
    init();
}

void SFMLRenderer::init() {
    window.setVerticalSyncEnabled(true);
    textFont.loadFromFile("OpenSans-Regular.ttf");
}

void SFMLRenderer::setTextColor(int r, int g, int b) {
    textColor.r = r;
    textColor.g = g;
    textColor.b = b;
}

void SFMLRenderer::drawText(float x, float y, const std::string& text, int size) {
    sf::Text t;
    t.setFont(textFont);
    t.setString(text);
    t.setCharacterSize(size);
    t.setColor(textColor);
    t.setPosition(x, y);
    sf::FloatRect textRect = t.getLocalBounds();
    t.setOrigin(textRect.width/2,textRect.height/2);
    window.draw(t);
}

void SFMLRenderer::updateDisplay() {
    window.display();
}

int SFMLRenderer::getDisplayWidth() {
    return window.getSize().x;
}

int SFMLRenderer::getDisplayHeight() {
    return window.getSize().y;
}

sf::Window& SFMLRenderer::getWindow() {
    return window;
}