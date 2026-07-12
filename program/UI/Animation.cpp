#include "Animation.h"

Animation::Animation(std::vector<SDL_Texture*> frames, std::vector<int> durations,
                     int loopStart, int loopEnd)
    : frames(frames), durations(durations), loopStart(loopStart), loopEnd(loopEnd) {
}

void Animation::update() {
    if (finished) return;

    frameTimer++;
    if (frameTimer >= durations[currentFrame]) {
        frameTimer = 0;
        currentFrame++;

        if (loop){
            if (currentFrame > loopEnd || currentFrame >=frames.size()) {
                currentFrame = loopStart;
            }
        }
        else{
            if(currentFrame >= frames.size()){
                finished=true;
            }
        }
    }
}

void Animation::draw(SDL_Renderer* renderer, int x, int y, int w, int h) {
    if (frames.empty()) return;
    SDL_Rect dst = {x, y, w, h};
    SDL_RenderCopy(renderer, frames[currentFrame], NULL, &dst);
}

void Animation::reset() {
    currentFrame = 0;
    frameTimer = 0;
    finished = false;
}