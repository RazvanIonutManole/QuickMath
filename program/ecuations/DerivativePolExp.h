#ifndef DerivativePolExp_H
#define DerivativePolExp_H

#include "Ecuation.h"
#include "../UI/MathLabel.h"
#include "../UI/PowerLabel.h"

#include <random>

class DerivativePolExp : public Ecuation {
private:
    MathLabel* hintLabel1 = nullptr;
    MathLabel* hintLabel2 = nullptr;
    PowerLabel* hintPower1 = nullptr;
    PowerLabel* hintPower2 = nullptr;
    MathLabel* ecuationLabel = nullptr;
    PowerLabel* ecuationPower = nullptr;
    MathLabel* dfx = nullptr;
    MathLabel* derivativeLabel = nullptr;
    PowerLabel* derivativePower = nullptr;
    MathLabel* summaryLabel = nullptr;
    PowerLabel* summaryPower = nullptr;
    MathLabel* answerLabel = nullptr;
    PowerLabel* answerPower = nullptr;
    MathLabel* correctionLabel = nullptr;
    PowerLabel* correctionPower = nullptr;
public:
    // Constructor
    DerivativePolExp(std::mt19937& engine);

    // Destructor (virtual because we are using inheritance)
    virtual ~DerivativePolExp();

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