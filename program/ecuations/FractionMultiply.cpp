#include "FractionMultiply.h"

FractionMultiply::FractionMultiply(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(4);
    correctAnswer.resize(2);
    generateProblem();
    calculateAnswer();
}

FractionMultiply::~FractionMultiply(){
        delete fracLabel1;
        delete fracLabel2;
        delete operatorLabel;

        delete hintLabel1;
        delete hintLabel2;
        delete hintLabel3;

        delete answerLabel1;
        delete operatorAnswerLabel;
        delete answerLabel2;
        delete equalsAnswerLabel;
        delete answerLabel3;

        delete correctionLabel1;
        delete operatorCorrectionLabel;
        delete correctionLabel2;
        delete equalsCorrectionLabel;
        delete correctionLabel3;
}

void FractionMultiply::calculateAnswer() {
    correctAnswer[0] = static_cast<double>(parameters[0] * parameters[2]);
    correctAnswer[1] = static_cast<double>(parameters[3] * parameters[1]);
}

void FractionMultiply::generateProblem() {
    std::uniform_int_distribution<int> dist(1, 10);
    parameters[0] = dist(engine); 
    parameters[1] = dist(engine);
    parameters[2] = dist(engine);
    parameters[3] = dist(engine);
}

void FractionMultiply::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    if (!fracLabel1) fracLabel1 = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    if (!fracLabel2) fracLabel2 = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    if (!operatorLabel) operatorLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    int centerX = 500;
    int centerY = 300;

    // space them out horizontally around the center
    fracLabel1->update(renderer, std::to_string(parameters[0]), std::to_string(parameters[1]), "", centerX - 100, centerY);
    operatorLabel->update(renderer, "*", centerX - 50, centerY);
    fracLabel2->update(renderer, std::to_string(parameters[2]), std::to_string(parameters[3]), "= ", centerX, centerY);

    if(!hintLabel1) hintLabel1 = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    if(!hintLabel2) hintLabel2 = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    if(!hintLabel3) hintLabel3 = new FractionLabel(FontSet.large, {0, 0, 0, 255});

    hintLabel1->update(renderer, "a", "b", " * ", hintX + 10, hintY);

    hintLabel2->update(renderer, "c", "d", " = ", hintX + 110, hintY);

    hintLabel3->update(renderer, "a*b", "c*d", "", hintX + 210, hintY);
}

void FractionMultiply::drawExtra(SDL_Renderer* renderer) {
    if (fracLabel1) fracLabel1->draw(renderer);
    if (operatorLabel) operatorLabel->draw(renderer);
    if (fracLabel2) fracLabel2->draw(renderer);

    if(hintChecked){
        if(hintLabel1) hintLabel1->draw(renderer);
        if(hintLabel2) hintLabel2->draw(renderer);
        if(hintLabel3) hintLabel3->draw(renderer);
    }

    // draw fraction bar for the answer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect line = { centerX+65, centerY, 70, 2 }; // centered between the two boxes
    SDL_RenderFillRect(renderer, &line);

}

std::vector<AnswerSlot> FractionMultiply::getAnswerSlots() {
    int boxW = 60;
    int boxH = 35;
    int gap = 8;

    return {
        {centerX + 70, centerY - gap - boxH, boxW, boxH, ""},  // numerator box
        {centerX + 70, centerY + gap,        boxW, boxH, ""}   // denominator box
    };
}

bool FractionMultiply::verifyAnswer(std::vector<double> answer) {
    if (answer.size() != 2) { wasCorrect = false; return false; }
    bool correct = std::abs(answer[0] * correctAnswer[1] - answer[1] * correctAnswer[0]) < 0.001;
    wasCorrect = correct;
    return correct;
}

void FractionMultiply::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }

    answerLabel1 = new FractionLabel{FontSet.large, color, renderer, 
                std::to_string(static_cast<int>(parameters[0])), 
                std::to_string(static_cast<int>(parameters[1])), 
                "", 0, 0};
    operatorAnswerLabel = new MathLabel(FontSet.large, color, renderer, "*", 0, 0);
    answerLabel2 = new FractionLabel{FontSet.large, color, renderer, 
                std::to_string(static_cast<int>(parameters[2])), 
                std::to_string(static_cast<int>(parameters[3])), "", 0, 0};
    equalsAnswerLabel = new MathLabel(FontSet.large, color, renderer, "=", 0, 0);
    answerLabel3 = new FractionLabel{FontSet.large, color, renderer, 
                std::to_string(static_cast<int>(givenAnswer[0])), 
                std::to_string(static_cast<int>(givenAnswer[1])), "", 0, 0};
    
    
    scrollBox->addRow({ answerLabel1, operatorAnswerLabel, answerLabel2, equalsAnswerLabel, answerLabel3 });

    if(!wasCorrect){
        correctionLabel1 = new FractionLabel{FontSet.large, {0, 0, 0, 255}, renderer, 
                        std::to_string(static_cast<int>(parameters[0])), 
                        std::to_string(static_cast<int>(parameters[1])),
                        "", 0, 0};
        operatorCorrectionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, "*", 0, 0);
        correctionLabel2 = new FractionLabel{FontSet.large, {0, 0, 0, 255}, renderer, 
                        std::to_string(static_cast<int>(parameters[2])), 
                        std::to_string(static_cast<int>(parameters[3])), "", 0, 0};
        equalsCorrectionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, "=", 0, 0);
        correctionLabel3 = new FractionLabel{FontSet.large, {0, 0, 0, 255}, renderer, 
                        std::to_string(static_cast<int>(correctAnswer[0])), 
                        std::to_string(static_cast<int>(correctAnswer[1])), "", 0, 0};
        
        scrollBox->addRow({ correctionLabel1, operatorCorrectionLabel, correctionLabel2, equalsCorrectionLabel, correctionLabel3 });
    }
}

void FractionMultiply::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }

    answerLabel1 = new FractionLabel{FontSet.large, color, renderer, 
                std::to_string(static_cast<int>(parameters[0])), 
                std::to_string(static_cast<int>(parameters[1])), 
                "", 0, 0};
    operatorAnswerLabel = new MathLabel(FontSet.large, color, renderer, "*", 0, 0);
    answerLabel2 = new FractionLabel{FontSet.large, color, renderer, 
                std::to_string(static_cast<int>(parameters[2])), 
                std::to_string(static_cast<int>(parameters[3])), "", 0, 0};
    equalsAnswerLabel = new MathLabel(FontSet.large, color, renderer, "=", 0, 0);
    answerLabel3 = new FractionLabel{FontSet.large, color, renderer, 
                std::to_string(static_cast<int>(givenAnswer[0])), 
                std::to_string(static_cast<int>(givenAnswer[1])), "", 0, 0};
    
    
    pageBox->addRow({ answerLabel1, operatorAnswerLabel, answerLabel2, equalsAnswerLabel, answerLabel3 });

    if(!wasCorrect){
        correctionLabel1 = new FractionLabel{FontSet.large, {0, 0, 0, 255}, renderer, 
                        std::to_string(static_cast<int>(parameters[0])), 
                        std::to_string(static_cast<int>(parameters[1])),
                        "", 0, 0};
        operatorCorrectionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, "*", 0, 0);
        correctionLabel2 = new FractionLabel{FontSet.large, {0, 0, 0, 255}, renderer, 
                        std::to_string(static_cast<int>(parameters[2])), 
                        std::to_string(static_cast<int>(parameters[3])), "", 0, 0};
        equalsCorrectionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, "=", 0, 0);
        correctionLabel3 = new FractionLabel{FontSet.large, {0, 0, 0, 255}, renderer, 
                        std::to_string(static_cast<int>(correctAnswer[0])), 
                        std::to_string(static_cast<int>(correctAnswer[1])), "", 0, 0};
        
        pageBox->addRow({ correctionLabel1, operatorCorrectionLabel, correctionLabel2, equalsCorrectionLabel, correctionLabel3 });
    }
}