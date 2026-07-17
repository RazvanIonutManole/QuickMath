#ifndef ImageButton_H
#define ImageButton_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Button.h"

class ImageButton : public Button {
public:
    SDL_Texture* normalTexture;
    SDL_Texture* pressedTexture;
    SDL_Texture* disabledTexture;
    bool isPressed = false;
    bool isEnabled = true;

    ImageButton(std::string Name, int x, int y, int w, int h, SDL_Texture* normalTex, SDL_Texture* pressedTex, SDL_Texture* disabledTexture);

    ~ImageButton();

    void draw(SDL_Renderer* renderer) override;

    bool isClicked(SDL_Event& event) override;

    bool handleEvent(SDL_Event& event);
};

#endif