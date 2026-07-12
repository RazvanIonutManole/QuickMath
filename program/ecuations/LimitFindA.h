#ifndef LimitFindA_H
#define LimitFindA_H

#include "Ecuation.h"
#include "MathLabel.h"
#include "FractionLabel.h"

#include <random>

class LimitFindA : public Ecuation {
private:
    MathLabel* hintLabel = nullptr;
    MathLabel* hintLabel2 = nullptr;
    MathLabel* hintLabel3 = nullptr;
    MathLabel* ecuationLabel = nullptr;
    FractionLabel* ecuationFunction = nullptr;
    MathLabel* ecuationQuestion = nullptr;
    MathLabel* answerLabel = nullptr;

    MathLabel* summaryLabel = nullptr;
    FractionLabel* summaryFunction = nullptr;
    MathLabel* summaryQuestion = nullptr;
    MathLabel* summaryAnswer = nullptr;
    MathLabel* correctionLabel = nullptr;
public:
    // Constructor
    LimitFindA(std::mt19937& engine);

    // Destructor (virtual because we are using inheritance)
    virtual ~LimitFindA();

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