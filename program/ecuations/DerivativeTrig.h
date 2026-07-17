#ifndef DerivativeTrig_H
#define DerivativeTrig_H

#include "Ecuation.h"
<<<<<<< HEAD
#include "../UI/MathLabel.h"
=======
#include "MathLabel.h"
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930
#include <random>

class DerivativeTrig : public Ecuation {
private:
    int order; // 0 = cos first, 1 = sin first
    MathLabel* fxLabel = nullptr;
    MathLabel* dfxLabel = nullptr;
    MathLabel* sinLabel = nullptr;  // "sin(Nx)" part
    MathLabel* cosLabel = nullptr;  // "cos(Nx)" part
    MathLabel* plusLabel = nullptr; // "+" or "-" between terms

    MathLabel* ecuationLabel = nullptr;
    MathLabel* answerLabel = nullptr;
    MathLabel* correctionLabel = nullptr;

    MathLabel* sinHintLabel = nullptr;
    MathLabel* cosHintLabel = nullptr;
    bool labelsBuilt = false;
public:
    // Constructor
    DerivativeTrig(std::mt19937& engine);

    // Destructor (virtual because we are using inheritance)
    virtual ~DerivativeTrig();

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