#ifndef SCROLLBOX_H
#define SCROLLBOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Label.h"
#include "MathLabel.h"
#include "FractionLabel.h"
#include "PowerLabel.h"

class ScrollBox {
public:
    ScrollBox(int x, int y, int w, int h, int rowHeight);
    ~ScrollBox();

    void addRow(std::vector<Label*> row);
    void clear(); // clears rows but does NOT delete labels - equations own them
    void scrollUp();
    void scrollDown();
    void draw(SDL_Renderer* renderer);
    void handleEvent(SDL_Event& e);

private:
    int x, y, w, h;
    int rowHeight;
    int scrollOffset = 0;
    int visibleRows;
    std::vector<std::vector<Label*>> rows;

    void repositionRow(int rowIndex);
};

#endif