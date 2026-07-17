#include "RadioGroup.h"

RadioGroup::RadioGroup() {}

RadioGroup::~RadioGroup() {
    for (auto& btn : buttons) {
        delete btn.labelObj;
    }
}

void RadioGroup::addButton(int x, int y, int w, int h,
                           SDL_Texture* normalTex, SDL_Texture* selectedTex,
                           std::string label, SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {
    RadioButton btn;
    btn.x = x;
    btn.y = y;
    btn.w = w;
    btn.h = h;
    btn.normalTexture = normalTex;
    btn.selectedTexture = selectedTex;
    btn.label = label;

    if (!label.empty() && font != nullptr) {
        btn.labelObj = new MathLabel(font, color);
        btn.labelObj->update(renderer, label, x + w / 2, y - 20);
    }

    buttons.push_back(btn);
}

void RadioGroup::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        for (int i = 0; i < (int)buttons.size(); i++) {
            auto& btn = buttons[i];
            SDL_Rect rect = {btn.x, btn.y, btn.w, btn.h};
            SDL_Point mouse = {mx, my};
            if (SDL_PointInRect(&mouse, &rect)) {
                selectedIndex = i;
                break;
            }
        }
    }
}

void RadioGroup::draw(SDL_Renderer* renderer) {
    for (int i = 0; i < (int)buttons.size(); i++) {
        auto& btn = buttons[i];
        SDL_Rect rect = {btn.x, btn.y, btn.w, btn.h};

        SDL_Texture* tex = (i == selectedIndex) ? btn.selectedTexture : btn.normalTexture;
        if (tex) SDL_RenderCopy(renderer, tex, NULL, &rect);

        if (btn.labelObj) btn.labelObj->draw(renderer);
    }
}

int RadioGroup::getSelected() const {
    return selectedIndex;
}