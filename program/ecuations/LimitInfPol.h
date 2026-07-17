#ifndef LimitInfPol_H
#define LimitInfPol_H

#include "Ecuation.h"
<<<<<<< HEAD
#include "../UI/MathLabel.h"
#include "../UI/FractionLabel.h"
=======
#include "MathLabel.h"
#include "FractionLabel.h"
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930

#include <random>

class LimitInfPol : public Ecuation {
private:
    MathLabel* hintLabel = nullptr;
    MathLabel* hintLabel2 = nullptr;
    FractionLabel* hintLabel3 = nullptr;
    FractionLabel* hintLabel4 = nullptr;
    FractionLabel* hintLim = nullptr;
    FractionLabel* hintFraction1 = nullptr;
    FractionLabel* hintLim2 = nullptr;
    FractionLabel* hintFraction2 = nullptr;

    FractionLabel* ecuationLim = nullptr;
    FractionLabel* ecuationFraction = nullptr;

    FractionLabel* summaryLim = nullptr;
    FractionLabel* summaryFraction = nullptr;
    MathLabel* summaryAnswer = nullptr;

    FractionLabel* correctionLim = nullptr;
    FractionLabel* correctionFraction = nullptr;
    MathLabel* correctionAnswer = nullptr;
public:
    // Constructor
    LimitInfPol(std::mt19937& engine);

    // Destructor (virtual because we are using inheritance)
    virtual ~LimitInfPol();

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