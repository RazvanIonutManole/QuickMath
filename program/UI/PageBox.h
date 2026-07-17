#ifndef PAGEBOX_H
#define PAGEBOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "Label.h"
#include "MathLabel.h"
#include "FontSet.h"

class PageBox {
public:
    PageBox(int x, int y, int w, int h, int rowHeight, FontSet fonts);
    ~PageBox();

    void addRow(std::vector<Label*> row);
    void nextQuestion(SDL_Renderer* renderer, std::string questionNumber); // call before adding rows for a new question
    void finalizePage();
    void clear();
    void nextPage();
    void prevPage();
    void draw(SDL_Renderer* renderer);
    void handleEvent(SDL_Event& e);

    int getCurrentPage() const { return currentPage; }
    int getTotalPages() const { return pages.size(); }

private:
    int x, y, w, h;
    int rowHeight;
    int currentPage = 0;
    FontSet fonts;

    // each page is a vector of rows, each row is a vector of Label*
    std::vector<std::vector<std::vector<Label*>>> pages;
    // current page being built
    std::vector<std::vector<Label*>> currentPageRows;

    MathLabel* pageNumberLabel = nullptr;
    MathLabel* questionLabel = nullptr;

    void repositionRow(int rowIndex, int pageIndex);
    void repositionCurrentPage();
};

#endif