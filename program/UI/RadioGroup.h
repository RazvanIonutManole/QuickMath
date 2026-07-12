#ifndef RADIOGROUP_H
#define RADIOGROUP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "MathLabel.h"

struct RadioButton {
    int x, y, w, h;
    SDL_Texture* normalTexture;
    SDL_Texture* selectedTexture;
    std::string label;
    MathLabel* labelObj = nullptr;
};

class RadioGroup {
public:
    std::vector<RadioButton> buttons;
    RadioGroup();
    ~RadioGroup();

    void addButton(int x, int y, int w, int h,
                   SDL_Texture* normalTex, SDL_Texture* selectedTex,
                   std::string label, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color);

    void handleEvent(SDL_Event& e);
    void draw(SDL_Renderer* renderer);
    int getSelected() const; // returns index of selected button, -1 if none

private:
    int selectedIndex = -1;
};

#endif