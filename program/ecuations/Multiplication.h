#ifndef MULTIPLICATION_H
#define MULTIPLICATION_H

#include "Ecuation.h"
<<<<<<< HEAD
#include "../UI/MathLabel.h"
=======
#include "MathLabel.h"
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930
#include <random>

class Multiplication : public Ecuation {
private:
    MathLabel* ecuationLabel = nullptr;
    MathLabel* hintLabel = nullptr;
    MathLabel* summaryLabel = nullptr;
    MathLabel* correctionLabel = nullptr;
public:
    // Constructor
    Multiplication(std::mt19937& engine);

    // Destructor (virtual because we are using inheritance)
    virtual ~Multiplication();

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