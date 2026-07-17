#ifndef DerivativePol_H
#define DerivativePol_H

#include "Ecuation.h"
<<<<<<< HEAD
#include "../UI/MathLabel.h"
#include "../UI/PowerLabel.h"
=======
#include "..\UI\MathLabel.h"
#include "..\UI\PowerLabel.h"
>>>>>>> 9249e509f5aa743e968d1056335f9becc9651930
#include <random>

class DerivativePol : public Ecuation {
private:
    int grade;

    // f(x) line labels
    MathLabel* fxLabel = nullptr;
    std::vector<PowerLabel*> fxPowerLabels;
    std::vector<MathLabel*> fxOperatorLabels;
    MathLabel* fxLinearLabel = nullptr;
    MathLabel* fxConstantLabel = nullptr;

    // f'(x) line labels
    MathLabel* dfxLabel = nullptr;
    std::vector<PowerLabel*> dfxPowerLabels;
    std::vector<MathLabel*> dfxOperatorLabels;
    MathLabel* dfxLinearLabel = nullptr;
    MathLabel* dfxConstantLabel = nullptr;

    //a*x^b = a*b*x^b-1
    MathLabel* hint1;
    PowerLabel* XToB;
    MathLabel* hint2;
    PowerLabel* XToB2;

    std::vector<Label*> summaryLabels;
    bool labelsBuilt = false;

public:
    DerivativePol(std::mt19937& engine);
    virtual ~DerivativePol();

    void calculateAnswer() override;
    void generateProblem() override;
    void buildDisplay(SDL_Renderer* renderer, FontSet FontSet) override;
    void drawExtra(SDL_Renderer* renderer) override;
    std::vector<AnswerSlot> getAnswerSlots() override;
    void addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) override;
    void addToPageBox(PageBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) override;
};

#endif