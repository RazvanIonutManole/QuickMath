#ifndef DIVISION_H
#define DIVISION_H

#include "Ecuation.h"
#include "../UI/MathLabel.h"
#include <random>

class Division : public Ecuation {
private:
    MathLabel* hintLabel = nullptr;
    MathLabel* ecuationLabel = nullptr;
    MathLabel* summaryLabel = nullptr;
    MathLabel* correctionLabel = nullptr;
public:
    // Constructor
    Division(std::mt19937& engine);

    // Destructor (virtual because we are using inheritance)
    virtual ~Division();

    // Logic Methods
    void calculateAnswer() override;
    void generateProblem() override;

    // Rendering Method
    void buildDisplay(SDL_Renderer* renderer, FontSet FontSet) override;
    std::vector<AnswerSlot> getAnswerSlots() override;
    void drawExtra(SDL_Renderer* renderer) override;
    void addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) override;
    void addToPageBox(PageBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) override;
};

#endif