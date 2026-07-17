#include "PowerLabel.h"

PowerLabel::PowerLabel(TTF_Font* baseFont, TTF_Font* superFont, SDL_Color color)
    : superFont(superFont) {
    baseLabel = new MathLabel(baseFont, color);
    exponentLabel = new MathLabel(superFont, color);
}

PowerLabel::PowerLabel(TTF_Font* baseFont, TTF_Font* superFont, SDL_Color color,
                       SDL_Renderer* renderer, std::string base, std::string exponent, int x, int y)
    : superFont(superFont) {
    baseLabel = new MathLabel(baseFont, color);
    exponentLabel = new MathLabel(superFont, color);
    update(renderer, base, exponent, x, y);

    updateBounds();
}

PowerLabel::PowerLabel(Label* base, Label* exponent)
    : superFont(nullptr) {
    baseLabel = base;
    exponentLabel = exponent;
}

PowerLabel::~PowerLabel() {
    delete baseLabel;
    delete exponentLabel;
}

void PowerLabel::update(SDL_Renderer* renderer, std::string base, std::string exponent, int x, int y) {
    // 1. Center-align the base label at the requested x
    auto* baseMath = static_cast<MathLabel*>(baseLabel);
    baseMath->update(renderer, base, x, y);
    
    // 2. Since the base is center-aligned, its right edge sits at: x + (width / 2)
    int exponentX = x + (baseMath->rect.w / 2);
    int superOffset = TTF_FontHeight(superFont) / 2;
    
    // 3. Render the exponent using left-alignment from that right edge
    auto* expMath = static_cast<MathLabel*>(exponentLabel);
    expMath->updateLeft(renderer, exponent, exponentX, y - superOffset);
    
    updateBounds();
}

void PowerLabel::updateLeft(SDL_Renderer* renderer, std::string base, std::string exponent, int x, int y) {
    // 1. Left-align the base label at the requested x
    auto* baseMath = static_cast<MathLabel*>(baseLabel);
    baseMath->updateLeft(renderer, base, x, y);
    
    // 2. Since the base is left-aligned, its right edge sits at: x + width
    int exponentX = x + baseMath->rect.w;
    int superOffset = TTF_FontHeight(superFont) / 2;
    
    // 3. Render the exponent using left-alignment from that right edge
    auto* expMath = static_cast<MathLabel*>(exponentLabel);
    expMath->updateLeft(renderer, exponent, exponentX, y - superOffset);
    
    updateBounds();
}

void PowerLabel::draw(SDL_Renderer* renderer) {
    if (baseLabel) baseLabel->draw(renderer);
    if (exponentLabel) exponentLabel->draw(renderer);
}

void PowerLabel::reposition(int x, int y) {
    // shift exponent relative to base's new position
    int dx = x - baseLabel->rect.x - baseLabel->rect.w / 2;
    int dy = y - baseLabel->rect.y - baseLabel->rect.h / 2;
    
    baseLabel->reposition(x, y);
    exponentLabel->reposition(
        exponentLabel->rect.x + exponentLabel->rect.w / 2 + dx,
        exponentLabel->rect.y + exponentLabel->rect.h / 2 + dy
    );
}

void PowerLabel::updateBounds() {
    if (!baseLabel || !exponentLabel) return;

    int left   = std::min(baseLabel->rect.x, exponentLabel->rect.x);
    int top    = std::min(baseLabel->rect.y, exponentLabel->rect.y);
    int right  = std::max(baseLabel->rect.x + baseLabel->rect.w, 
                          exponentLabel->rect.x + exponentLabel->rect.w);
    int bottom = std::max(baseLabel->rect.y + baseLabel->rect.h, 
                          exponentLabel->rect.y + exponentLabel->rect.h);

    rect = {left, top, right - left, bottom - top};
}