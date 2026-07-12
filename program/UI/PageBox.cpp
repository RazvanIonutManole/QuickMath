#include "PageBox.h"

PageBox::PageBox(int x, int y, int w, int h, int rowHeight, FontSet fonts)
    : x(x), y(y), w(w), h(h), rowHeight(rowHeight), fonts(fonts) {
    pageNumberLabel = new MathLabel(fonts.medium, {0, 0, 0, 255});
    questionLabel = new MathLabel(fonts.medium, {0, 0, 0, 255});
}

PageBox::~PageBox() {
    delete pageNumberLabel;
    delete questionLabel;
    // labels owned by equations, don't delete them here
}

void PageBox::nextQuestion(SDL_Renderer* renderer, std::string questionNumber) {
    if (!currentPageRows.empty()) {
        pages.push_back(currentPageRows);
        currentPageRows.clear();
    }

    auto* qLabel = new MathLabel(fonts.medium, {0, 0, 0, 255}, renderer,
                                  "Question " + questionNumber, 0, 0);
    currentPageRows.push_back({qLabel});
}

void PageBox::addRow(std::vector<Label*> row) {
    currentPageRows.push_back(row);
}

void PageBox::clear() {
    pages.clear();
    currentPageRows.clear();
    currentPage = 0;
}

void PageBox::nextPage() {
    if (currentPage < (int)pages.size() - 1) {
        currentPage++;
    }
}

void PageBox::prevPage() {
    if (currentPage > 0) {
        currentPage--;
    }
}

void PageBox::repositionRow(int rowIndex, int pageIndex) {
    int padding = rowHeight / 2;
    int rowY = y + padding + rowIndex * rowHeight;
    auto& row = pages[pageIndex][rowIndex];

    if (row.empty()) return;

    if (row.size() == 1) {
        row[0]->reposition(x + w / 2, rowY);
        return;
    }

    int totalLabelWidth = 0;
    for (auto* label : row)
        totalLabelWidth += label->rect.w;

    int totalGapSpace = w - 40 - totalLabelWidth;
    int gap = totalGapSpace / ((int)row.size() + 1);

    int currentX = x + 20 + gap;
    for (auto* label : row) {
        label->reposition(currentX + label->rect.w / 2, rowY);
        currentX += label->rect.w + gap;
    }
}

void PageBox::repositionCurrentPage() {
    for (int i = 0; i < (int)pages[currentPage].size(); i++) {
        repositionRow(i, currentPage);
    }
}

// draw left arrow triangle
void drawLeftArrow(SDL_Renderer* renderer, int cx, int cy, int size) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < size; i++) {
        int halfWidth = i;
        SDL_RenderDrawLine(renderer, cx - size + i, cy - halfWidth, cx - size + i, cy + halfWidth);
    }
}

// draw right arrow triangle
void drawRightArrow(SDL_Renderer* renderer, int cx, int cy, int size) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < size; i++) {
        int halfWidth = size - i;
        SDL_RenderDrawLine(renderer, cx + i, cy - halfWidth, cx + i, cy + halfWidth);
    }
}

void PageBox::draw(SDL_Renderer* renderer) {
    // background
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_Rect bgRect = {x, y, w, h};
    //SDL_RenderFillRect(renderer, &bgRect);

    // border
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //SDL_RenderDrawRect(renderer, &bgRect);

    if (pages.empty()) return;

    // reposition and draw current page rows
    repositionCurrentPage();
    for (auto& row : pages[currentPage]) {
        for (auto* label : row) {
            label->draw(renderer);
        }
    }

    // page indicator at bottom
    std::string pageText = "< " + std::to_string(currentPage + 1) + " / " +
                           std::to_string(pages.size()) + " >";
    pageNumberLabel->update(renderer, pageText, x + w / 2, y + h - 20);
    pageNumberLabel->draw(renderer);

    // prev/next arrows
    if (currentPage > 0)
        drawLeftArrow(renderer, x + 20, y + h - 20, 15);
    if (currentPage < (int)pages.size() - 1)
        drawRightArrow(renderer, x + w - 20, y + h - 20, 15);
    }

void PageBox::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_LEFT)  prevPage();
        if (e.key.keysym.sym == SDLK_RIGHT) nextPage();
    }
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // left arrow click area
        SDL_Rect leftArea = {x + 10, y + h - 30, 20, 20};
        SDL_Point mouse = {mx, my};
        if (SDL_PointInRect(&mouse, &leftArea)) prevPage();

        // right arrow click area
        SDL_Rect rightArea = {x + w - 30, y + h - 30, 20, 20};
        if (SDL_PointInRect(&mouse, &rightArea)) nextPage();
    }
}

void PageBox::finalizePage() {
    if (!currentPageRows.empty()) {
        pages.push_back(currentPageRows);
        currentPageRows.clear();
    }
}