#ifndef PolinomialG1_H
#define PolinomialG1_H

#include "Ecuation.h"
#include "../UI/MathLabel.h"
#include "../UI/FractionLabel.h"
#include <random>

class PolinomialG1 : public Ecuation {
private:
    MathLabel* ecuationLabel = nullptr;
    MathLabel* ecLabel = nullptr;
    MathLabel* answerLabel = nullptr;
    MathLabel* correctionLabel = nullptr;
public:
    // Constructor
    PolinomialG1(std::mt19937& engine);

    //ax+b=c x=(c-b)/a

    MathLabel* hintLabel = nullptr;
    MathLabel* hintLabel1 = nullptr;
    FractionLabel* hintLabel2 = nullptr;

    // Destructor (virtual because we are using inheritance)
    virtual ~PolinomialG1();

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