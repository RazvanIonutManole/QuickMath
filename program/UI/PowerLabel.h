#pragma once
#include "MathLabel.h"
#include "Label.h"
#include <string>

class PowerLabel : public Label {
public:
    // old constructor - builds MathLabels internally, use update() to set text
    PowerLabel(TTF_Font* baseFont, TTF_Font* superFont, SDL_Color color);

    // string constructor - builds and positions in one call
    PowerLabel(TTF_Font* baseFont, TTF_Font* superFont, SDL_Color color,
               SDL_Renderer* renderer, std::string base, std::string exponent, int x, int y);

    // mix-and-match constructor - accepts any Label*, positioning handled externally
    PowerLabel(Label* base, Label* exponent);

    ~PowerLabel();

    // only valid when using font-based constructors
    void update(SDL_Renderer* renderer, std::string base, std::string exponent, int x, int y);
    void updateLeft(SDL_Renderer* renderer, std::string base, std::string exponent, int x, int y);

    void draw(SDL_Renderer* renderer) override;
    void reposition(int x, int y) override;
    void updateBounds();

private:
    Label* baseLabel = nullptr;
    Label* exponentLabel = nullptr;
    TTF_Font* superFont = nullptr;
};