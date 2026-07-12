#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SDL2/SDL.h>
#include <string>
#include "MathLabel.h"

class TextBox {
public:
    SDL_Rect rect;
    std::string userInput;
    bool isFocused = false;
    int maxLength = 6;
    std::string hint = "";
    std::string allowedChars = "0123456789.-"; // default: numbers only
    bool masked = false;

    TextBox(int x, int y, int w, int h, TTF_Font* font, SDL_Color color);
    TextBox(int x, int y, int w, int h, TTF_Font* font, SDL_Color color, std::string hint);
    TextBox(int x, int y, int w, int h, TTF_Font* font, SDL_Color color, 
            std::string hint, std::string allowedChars, int maxLength);
    void handleEvent(SDL_Event& e);
    void draw(SDL_Renderer* renderer);
    void clearInput();
    void initHint(SDL_Renderer* renderer);

private:
    MathLabel* label;
    MathLabel* hintLabel;
    std::string lastInput = "";
};

#endif