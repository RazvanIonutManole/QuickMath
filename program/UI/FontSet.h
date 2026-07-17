#ifndef FONTSET_H
#define FONTSET_H

#include <SDL2/SDL_ttf.h>

struct FontSet {
    TTF_Font* large = nullptr;
    TTF_Font* medium = nullptr;
    TTF_Font* small = nullptr;
    TTF_Font* largeSuper = nullptr;
    TTF_Font* mediumSuper = nullptr;
    TTF_Font* smallSuper = nullptr;
};

extern FontSet fonts; // declares the global without defining it

#endif