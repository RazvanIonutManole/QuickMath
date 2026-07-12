#include "FractionLabel.h"
#include "MathLabel.h"

FractionLabel::FractionLabel(TTF_Font* font, SDL_Color color) {
    numeratorLabel = new MathLabel(font, color);
    denominatorLabel = new MathLabel(font, color);
    suffixLabel = new MathLabel(font, color);
    ownsLabels = true;
}

FractionLabel::FractionLabel(Label* numerator, Label* denominator, Label* suffix, int centerX, int centerY) {
    numeratorLabel = numerator;
    denominatorLabel = denominator;
    suffixLabel = suffix;
    ownsLabels = true; // caller passes ownership
    int lineW = std::max(numeratorLabel->rect.w, denominatorLabel->rect.w) + 10;
    lineRect = { centerX - lineW / 2, centerY - 1, lineW, 2 };
    updateBounds();
}

FractionLabel::FractionLabel(TTF_Font* font, SDL_Color color, SDL_Renderer* renderer,
                            std::string numerator, std::string denominator, std::string suffix,
                            int centerX, int centerY) {
    numeratorLabel = new MathLabel(font, color);
    denominatorLabel = new MathLabel(font, color);
    suffixLabel = new MathLabel(font, color);
    ownsLabels = true;
    update(renderer, numerator, denominator, suffix, centerX, centerY);

    updateBounds();
}

FractionLabel::~FractionLabel() {
    if (ownsLabels) {
        delete numeratorLabel;
        delete denominatorLabel;
        delete suffixLabel;
    }
}

void FractionLabel::update(SDL_Renderer* renderer,
                           std::string numerator, std::string denominator,
                           std::string suffix,
                           int centerX, int centerY) {
    // only works when using the font-based constructor
    MathLabel* num = static_cast<MathLabel*>(numeratorLabel);
    MathLabel* den = static_cast<MathLabel*>(denominatorLabel);
    MathLabel* suf = static_cast<MathLabel*>(suffixLabel);

    int lineHeight = TTF_FontHeight(num->font);
    int gap = 4;

    num->update(renderer, numerator, centerX, centerY - gap - lineHeight / 2);
    den->update(renderer, denominator, centerX, centerY + gap + lineHeight / 2);

    updateBounds();

    int lineW = std::max(numeratorLabel->rect.w, denominatorLabel->rect.w) + 10;
    lineRect = { centerX - lineW / 2, centerY - 1, lineW, 2 };

    suf->updateLeft(renderer, suffix, centerX + lineW / 2 + 15, centerY);

    updateBounds();

    
}

void FractionLabel::draw(SDL_Renderer* renderer) {
    if (numeratorLabel) numeratorLabel->draw(renderer);
    if (denominatorLabel) denominatorLabel->draw(renderer);
    if (suffixLabel) suffixLabel->draw(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if(drawLine)
        SDL_RenderFillRect(renderer, &lineRect);
}

void FractionLabel::reposition(int x, int y) {
    // x, y is the new center (where the line sits)
    int oldCenterX = lineRect.x + lineRect.w / 2;
    int oldCenterY = lineRect.y + 1;
    
    int dx = x - oldCenterX;
    int dy = y - oldCenterY;
    
    lineRect.x += dx;
    lineRect.y += dy;
    
    numeratorLabel->reposition(
        numeratorLabel->rect.x + numeratorLabel->rect.w / 2 + dx,
        numeratorLabel->rect.y + numeratorLabel->rect.h / 2 + dy
    );
    denominatorLabel->reposition(
        denominatorLabel->rect.x + denominatorLabel->rect.w / 2 + dx,
        denominatorLabel->rect.y + denominatorLabel->rect.h / 2 + dy
    );
    suffixLabel->reposition(
        suffixLabel->rect.x + suffixLabel->rect.w / 2 + dx,
        suffixLabel->rect.y + suffixLabel->rect.h / 2 + dy
    );
}

void FractionLabel::updateBounds() {
    int left = lineRect.x;
    int right = lineRect.x + lineRect.w;
    int top = lineRect.y;
    int bottom = lineRect.y + lineRect.h;

    // expand to include numerator
    if (numeratorLabel) {
        left   = std::min(left,   numeratorLabel->rect.x);
        right  = std::max(right,  numeratorLabel->rect.x + numeratorLabel->rect.w);
        top    = std::min(top,    numeratorLabel->rect.y);
        bottom = std::max(bottom, numeratorLabel->rect.y + numeratorLabel->rect.h);
    }
    // expand to include denominator
    if (denominatorLabel) {
        left   = std::min(left,   denominatorLabel->rect.x);
        right  = std::max(right,  denominatorLabel->rect.x + denominatorLabel->rect.w);
        top    = std::min(top,    denominatorLabel->rect.y);
        bottom = std::max(bottom, denominatorLabel->rect.y + denominatorLabel->rect.h);
    }
    // expand to include suffix
    if (suffixLabel) {
        left   = std::min(left,   suffixLabel->rect.x);
        right  = std::max(right,  suffixLabel->rect.x + suffixLabel->rect.w);
        top    = std::min(top,    suffixLabel->rect.y);
        bottom = std::max(bottom, suffixLabel->rect.y + suffixLabel->rect.h);
    }

    rect = {left, top, right - left, bottom - top};
}