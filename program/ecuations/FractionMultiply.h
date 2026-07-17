#ifndef FractionMultiply_H
#define FractionMultiply_H

#include "Ecuation.h"
<<<<<<< HEAD
#include "../UI/MathLabel.h"
#include "../UI/FractionLabel.h"
=======
#include "MathLabel.h"
#include "FractionLabel.h"
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930
#include <random>

class FractionMultiply : public Ecuation {
private:
    FractionLabel* fracLabel1 = nullptr;
    FractionLabel* fracLabel2 = nullptr;
    MathLabel* operatorLabel = nullptr;

    FractionLabel* hintLabel1 = nullptr;
    FractionLabel* hintLabel2 = nullptr;
    FractionLabel* hintLabel3 = nullptr;

    FractionLabel* answerLabel1 = nullptr;
    MathLabel* operatorAnswerLabel = nullptr;
    FractionLabel* answerLabel2 = nullptr;
    MathLabel* equalsAnswerLabel = nullptr;
    FractionLabel* answerLabel3 = nullptr;
    FractionLabel* correctionLabel1 = nullptr;
    MathLabel* operatorCorrectionLabel = nullptr;
    FractionLabel* correctionLabel2 = nullptr;
    MathLabel* equalsCorrectionLabel = nullptr;
    FractionLabel* correctionLabel3 = nullptr;
public:
    
    // Constructor
    FractionMultiply(std::mt19937& engine);

    // Destructor (virtual because we are using inheritance)
    virtual ~FractionMultiply();

    // Logic Methods
    void calculateAnswer() override;
    void generateProblem() override;

    // Rendering Method
    void buildDisplay(SDL_Renderer* renderer, FontSet FontSet) override;
    std::vector<AnswerSlot> getAnswerSlots() override;
    bool verifyAnswer(std::vector<double> answer) override;
    void drawExtra(SDL_Renderer* renderer) override;
    void addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) override;
    void addToPageBox(PageBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) override;
};

#endif