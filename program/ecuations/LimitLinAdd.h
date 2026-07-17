#ifndef LimitLinAdd_H
#define LimitLinAdd_H

#include "Ecuation.h"
<<<<<<< HEAD
#include "../UI/MathLabel.h"
#include "../UI/FractionLabel.h"
=======
#include "MathLabel.h"
#include "FractionLabel.h"
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930

#include <random>

class LimitLinAdd : public Ecuation {
private:
    MathLabel* hintLabel = nullptr;
    MathLabel* ecuationLabel = nullptr;
    FractionLabel* ecuationLim = nullptr;
    MathLabel* summaryLabel = nullptr;
    FractionLabel* summaryLim = nullptr;
    MathLabel* correctionLabel = nullptr;
    FractionLabel* correctionLim = nullptr;
public:
    // Constructor
    LimitLinAdd(std::mt19937& engine);

    // Destructor (virtual because we are using inheritance)
    virtual ~LimitLinAdd();

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