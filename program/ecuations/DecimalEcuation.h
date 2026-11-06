#ifndef DecimalEcuation_H
#define DecimalEcuation_H

#include "Ecuation.h"
#include "../UI/MathLabel.h"
#include <format>
#include <random>

class DecimalEcuation : public Ecuation {
private:
    MathLabel* hintLabel = nullptr;
    MathLabel* hintLabel2 = nullptr;
    MathLabel* ecuationLabel = nullptr;
    MathLabel* summaryLabel = nullptr;
    MathLabel* correctionLabel = nullptr;
public:
   
    // Constructor
    DecimalEcuation(std::mt19937& engine);

    // Destructor (virtual because we are using inheritance)
    virtual ~DecimalEcuation();

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