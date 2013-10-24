//
//  SFMLRenderer.h
//  steeringwheel
//
//  Created by Hannes Widmoser on 10/24/13.
//  Copyright (c) 2013 Hannes Widmoser. All rights reserved.
//

#ifndef __steeringwheel__SFMLRenderer__
#define __steeringwheel__SFMLRenderer__

#include <iostream>
#include <SFML/Window.hpp>
#include <engine/Renderer.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class SFMLRenderer : public Renderer {
public:
    SFMLRenderer(int width, int height, bool fullScreen = false);
    SFMLRenderer();
    
    void init();
    
    virtual void setTextColor(int r, int g, int b);
    virtual void drawText(float x, float y, const std::string& text, int size);
    virtual void updateDisplay();
    
    virtual int getDisplayWidth();
    virtual int getDisplayHeight();
    
    sf::Window& getWindow();
private:
    sf::RenderWindow window;
    sf::Font textFont;
    sf::Color textColor;
};

#endif /* defined(__steeringwheel__SFMLRenderer__) */
