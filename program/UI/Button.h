#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <string>

class Button {
protected:
    
    bool isHovered;
    
public:
    SDL_Rect rect;
    std::string name;
    Button(std::string Name, int x, int y, int w, int h) {
        name=Name;
        rect = {x, y, w, h};
        isHovered = false;
    }

    virtual ~Button() {}

    void updateHover(int mouseX, int mouseY) {
        SDL_Point mousePos = {mouseX, mouseY};
        isHovered = SDL_PointInRect(&mousePos, &rect);
    }

    virtual void draw(SDL_Renderer* renderer) = 0;
    
    virtual bool isClicked(SDL_Event& event) = 0;
};

#endif