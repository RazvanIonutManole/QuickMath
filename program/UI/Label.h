#ifndef LABEL_H
#define LABEL_H

#include <SDL2/SDL.h>

class Label {
public:
    SDL_Rect rect = {0, 0, 0, 0};
    
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual ~Label() {}
    virtual void reposition(int x, int y) = 0;
};

#endif