#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

class Animation {
public:
    bool loop = true;
    bool finished = false;

    Animation(std::vector<SDL_Texture*> frames, std::vector<int> durations,
              int loopStart, int loopEnd);

    void update();
    void draw(SDL_Renderer* renderer, int x, int y, int w, int h);
    void reset();

private:
    std::vector<SDL_Texture*> frames;
    std::vector<int> durations;
    int loopStart;
    int loopEnd;

    int currentFrame = 0;
    int frameTimer = 0;
};

#endif