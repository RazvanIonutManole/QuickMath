#ifndef MathLabel_H
#define MathLabel_H

#include <SDL2/SDL.h>
#include <string>
#include <SDL2\SDL_ttf.h>
#include "Label.h"

class MathLabel : public Label {
public:
    SDL_Texture* texture = nullptr;
    TTF_Font* font;
    SDL_Color color;

    MathLabel(TTF_Font* f, SDL_Color c);
    MathLabel(TTF_Font* f, SDL_Color c, SDL_Renderer* renderer, std::string text, int x, int y);

    ~MathLabel();

    void update(SDL_Renderer* renderer, std::string newText, int x, int y);
    void updateLeft(SDL_Renderer* renderer, std::string newText, int x, int y);
    void draw(SDL_Renderer* renderer) override;
    void reposition(int x, int y) override;
};

#endif