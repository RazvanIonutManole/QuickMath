#include "TextBox.h"

TextBox::TextBox(int x, int y, int w, int h, TTF_Font* font, SDL_Color color) {
    rect = {x, y, w, h};
    label = new MathLabel(font, color);
<<<<<<< HEAD
    lastInput = "/x01";
=======
    lastInput = "\x01";
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930
}

TextBox::TextBox(int x, int y, int w, int h, TTF_Font* font, SDL_Color color, std::string hint)
    : hint(hint) {
    rect = {x, y, w, h};
    label = new MathLabel(font, color);
<<<<<<< HEAD
    lastInput = "/x01";
=======
    lastInput = "\x01";
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930
    hintLabel = new MathLabel(font, color);
}

TextBox::TextBox(int x, int y, int w, int h, TTF_Font* font, SDL_Color color,
                 std::string hint, std::string allowedChars, int maxLength)
    : hint(hint), allowedChars(allowedChars), maxLength(maxLength) {
    rect = {x, y, w, h};
    label = new MathLabel(font, color);
<<<<<<< HEAD
    lastInput = "/x01";
=======
    lastInput = "\x01";
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930
    hintLabel = new MathLabel(font, color);
}

void TextBox::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        isFocused = (mx >= rect.x && mx <= rect.x + rect.w &&
                     my >= rect.y && my <= rect.y + rect.h);
        if (isFocused) SDL_StartTextInput();
    }

    if (!isFocused) return;

    if (e.type == SDL_TEXTINPUT) {
        if ((int)userInput.length() < maxLength) {
            std::string incoming = e.text.text;
            for (char c : incoming) {
                if (allowedChars.empty() || allowedChars.find(c) != std::string::npos) {
                    userInput += c;
                }
            }
        }
    }
    else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_BACKSPACE && !userInput.empty()) {
            userInput.pop_back();
        }
    }
}

void TextBox::draw(SDL_Renderer* renderer) {
    if (!hint.empty()) hintLabel->draw(renderer);

    // Draw border (slightly larger rect behind the white box)
    if (isFocused) {
        SDL_SetRenderDrawColor(renderer, 0, 120, 215, 255); // blue
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // gray
    }
    SDL_Rect borderRect = { rect.x - 4, rect.y - 4, rect.w + 8, rect.h + 8 };
    SDL_RenderFillRect(renderer, &borderRect);

    // Draw white fill on top
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    if (userInput != lastInput) {
        std::string display = masked ? std::string(userInput.size(), '*') : userInput;
        label->update(renderer, display, rect.x + rect.w / 2, rect.y + rect.h / 2);
        lastInput = userInput;
    }

    label->draw(renderer);
}

void TextBox::clearInput() {
    userInput = "";
<<<<<<< HEAD
    lastInput = "/x01";
=======
    lastInput = "\x01";
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930
}

void TextBox::initHint(SDL_Renderer* renderer) {
    if (!hint.empty())
        hintLabel->update(renderer, hint, rect.x - 40, rect.y + rect.h / 2);
}