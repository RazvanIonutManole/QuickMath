#include "MathLabel.h"

MathLabel::MathLabel(TTF_Font* f, SDL_Color c) : font(f), color(c) {
}

MathLabel::MathLabel(TTF_Font* f, SDL_Color c, SDL_Renderer* renderer, std::string text, int x, int y)
    : font(f), color(c) {
    update(renderer, text, x, y);
}

MathLabel::~MathLabel() {
    if (texture) SDL_DestroyTexture(texture);
}

void MathLabel::update(SDL_Renderer* renderer, std::string newText, int x, int y) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    if (newText.empty()) return;

    SDL_Surface* surf = TTF_RenderText_Blended(font, newText.c_str(), color);
    if (surf) {
        texture = SDL_CreateTextureFromSurface(renderer, surf);
        rect = { x - surf->w / 2, y - surf->h / 2, surf->w, surf->h };
        SDL_FreeSurface(surf);
    }
}

void MathLabel::updateLeft(SDL_Renderer* renderer, std::string newText, int x, int y) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    if (newText.empty()) return;

    SDL_Surface* surf = TTF_RenderText_Blended(font, newText.c_str(), color);
    if (surf) {
        texture = SDL_CreateTextureFromSurface(renderer, surf);
        rect = { x, y - surf->h / 2, surf->w, surf->h };
        SDL_FreeSurface(surf);
    }
}

void MathLabel::draw(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

void MathLabel::reposition(int x, int y) {
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;
}