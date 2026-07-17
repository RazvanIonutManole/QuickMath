#include "ScrollBox.h"

ScrollBox::ScrollBox(int x, int y, int w, int h, int rowHeight)
    : x(x), y(y), w(w), h(h), rowHeight(rowHeight) {
    visibleRows = h / rowHeight - 1;
}

ScrollBox::~ScrollBox() {
    // don't delete labels - equations own them
}

void ScrollBox::addRow(std::vector<Label*> row) {
    rows.push_back(row);
    repositionRow(rows.size() - 1);
}

void ScrollBox::clear() {
    rows.clear();
    scrollOffset = 0;
}

void ScrollBox::scrollUp() {
    if (scrollOffset > 0) {
        scrollOffset--;
        for (int i = scrollOffset; i < std::min(scrollOffset + visibleRows, (int)rows.size()); i++)
            repositionRow(i);
    }
}

void ScrollBox::scrollDown() {
    if (scrollOffset + visibleRows < (int)rows.size()) {
        scrollOffset++;
        for (int i = scrollOffset; i < std::min(scrollOffset + visibleRows, (int)rows.size()); i++)
            repositionRow(i);
    }
}

void ScrollBox::repositionRow(int rowIndex) {
    int padding = rowHeight; // changed from rowHeight / 2 to rowHeight
    int rowY = y + padding + (rowIndex - scrollOffset) * rowHeight;

    auto& row = rows[rowIndex];
    if (row.empty()) return;

    if (row.size() == 1) {
        // center single label
        row[0]->reposition(x + w / 2, rowY);
        return;
    }

    int totalLabelWidth = 0;
    for (auto* label : row)
        totalLabelWidth += label->rect.w;

    int totalGapSpace = w - 40 - totalLabelWidth;
    int gap = totalGapSpace / (row.size() + 1);

    int currentX = x + 20 + gap;
    for (auto* label : row) {
        label->reposition(currentX + label->rect.w / 2, rowY);
        currentX += label->rect.w + gap;
    }
}

void ScrollBox::draw(SDL_Renderer* renderer) {
    // background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect bgRect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &bgRect);

    // border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &bgRect);

    // visible rows
    int end = std::min(scrollOffset + visibleRows, (int)rows.size());
    for (int i = scrollOffset; i < end; i++) {
        for (auto* label : rows[i]) {
            label->draw(renderer);
        }
    }

    // scroll up indicator
    if (scrollOffset > 0) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect upRect = {x + w / 2 - 10, y + 5, 20, 10};
        SDL_RenderFillRect(renderer, &upRect);
    }

    // scroll down indicator
    if (scrollOffset + visibleRows < (int)rows.size()) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect downRect = {x + w / 2 - 10, y + h - 15, 20, 10};
        SDL_RenderFillRect(renderer, &downRect);
    }
}

void ScrollBox::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEWHEEL) {
        if (e.wheel.y > 0) scrollUp();
        if (e.wheel.y < 0) scrollDown();
    }
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_UP) scrollUp();
        if (e.key.keysym.sym == SDLK_DOWN) scrollDown();
    }
}