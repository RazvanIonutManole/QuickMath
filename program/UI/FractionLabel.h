#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Label.h"
#include <string>

class FractionLabel : public Label {
public:
    bool drawLine=true;
    // old constructor for existing equations
    FractionLabel(TTF_Font* font, SDL_Color color);

    // new constructor accepting any Label* for numerator/denominator/suffix
    FractionLabel(Label* numerator, Label* denominator, Label* suffix, int centerX, int centerY);

    FractionLabel(TTF_Font* font, SDL_Color color, SDL_Renderer* renderer,
                std::string numerator, std::string denominator, std::string suffix,
                int centerX, int centerY);

    ~FractionLabel();

    // old update for existing equations
    void update(SDL_Renderer* renderer,
                std::string numerator, std::string denominator,
                std::string suffix,
                int centerX, int centerY);

    void draw(SDL_Renderer* renderer) override;
    void reposition(int x, int y) override;
    void updateBounds();

private:
    Label* numeratorLabel = nullptr;
    Label* denominatorLabel = nullptr;
    Label* suffixLabel = nullptr;
    SDL_Rect lineRect;
    bool ownsLabels = true; // false when labels passed in from outside
};