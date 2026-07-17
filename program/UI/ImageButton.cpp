#include "ImageButton.h"

ImageButton::ImageButton(std::string Name, int x, int y, int w, int h, SDL_Texture* normalTex, SDL_Texture* pressedTex, SDL_Texture* disabledTex)
    : Button(Name, x, y, w, h) 
{
    normalTexture = normalTex;
    pressedTexture = pressedTex;
    disabledTexture = disabledTex;
}

ImageButton::~ImageButton() {
    SDL_DestroyTexture(normalTexture);
    SDL_DestroyTexture(pressedTexture);
}

void ImageButton::draw(SDL_Renderer* renderer) {
    SDL_Texture* activeTexture;
    if (isEnabled) {
        activeTexture = isPressed ? pressedTexture : normalTexture;
    }
    else activeTexture = disabledTexture;
    
    
    if (activeTexture) {
        SDL_Rect drawRect;
        drawRect.w = 100;
        drawRect.h = 100;
        drawRect.x = rect.x; 
        drawRect.y = (rect.y + rect.h) - drawRect.h;

        SDL_RenderCopy(renderer, activeTexture, NULL, &drawRect);
    }
}

bool ImageButton::isClicked(SDL_Event& event) {
    if (!isEnabled) return false;

    if (isHovered) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            isPressed = true;
        }
        if (event.type == SDL_MOUSEBUTTONUP && isPressed) {
            isPressed = false;
            return true;
        }
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
        isPressed = false;
    }
    
    return false;
}

bool ImageButton::handleEvent(SDL_Event& event) {
    return isClicked(event);
}