#include "PolinomialG1.h"

PolinomialG1::PolinomialG1(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(3);
    correctAnswer.resize(1);
    generateProblem();
}

void PolinomialG1::calculateAnswer() {
    correctAnswer[0] = static_cast<double>(parameters[2] - parameters[1]) / parameters[0];
}

PolinomialG1::~PolinomialG1(){
    delete(hintLabel);
    delete(ecuationLabel);
    delete(ecLabel);
    delete(correctionLabel);
}

void PolinomialG1::generateProblem() { //ax+b=c x=(c-b)/a
    std::uniform_int_distribution<int> ADist(2, 10);
    std::uniform_int_distribution<int> answerDist(2, 25);

    int a = ADist(engine);
    int answer = answerDist(engine);

    parameters[1] = a;
    std::uniform_int_distribution<int> BDist(1, a * answer);

    int b = BDist(engine);
    int c = a * answer + b;

    parameters[0] = a;
    parameters[1] = b;
    parameters[2] = c;
    correctAnswer[0] = static_cast<double>(answer);
}

void PolinomialG1::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    std::string text = std::to_string(parameters[0]) + " * x " + " + " + std::to_string(parameters[1]) + " = " + std::to_string(parameters[2]);
    ecuationLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLabel->update(renderer, text, centerX, centerY);

    if(!hintLabel) hintLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    if(!hintLabel1) hintLabel1 = new MathLabel(FontSet.large, {0, 0, 0, 255});
    if(!hintLabel2) hintLabel2 = new FractionLabel(FontSet.large, {0, 0, 0, 255});

    hintLabel->updateLeft(renderer, "a * x + b = c", hintX, hintY);
    hintLabel1->updateLeft(renderer, "x = ", hintX, hintY + 50);
    hintLabel2->update(renderer, "c - b", "a", "", hintX + 70, hintY + 50);
}

void PolinomialG1::drawExtra(SDL_Renderer* renderer){
    if(ecuationLabel) ecuationLabel->draw(renderer);
    if(hintChecked){
        if(hintLabel) hintLabel->draw(renderer);
        if(hintLabel1) hintLabel1->draw(renderer);
        if(hintLabel2) hintLabel2->draw(renderer);
    }
}

std::vector<AnswerSlot> PolinomialG1::getAnswerSlots() {
    return { {450, 400, 100, 50, "x ="} }; // one box
}

void PolinomialG1::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result, ec;
    SDL_Color color;
    ec = std::to_string(parameters[0]) + " * x " + " + " + std::to_string(parameters[1]) + " = " + std::to_string(parameters[2]);
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }

    ecLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, ec, 0, 0);
    
    scrollBox->addRow({ ecLabel });

    result = "x = " + std::to_string(static_cast<int>(givenAnswer[0]));
    answerLabel = new MathLabel(FontSet.large, color, renderer, result, 0, 0);

    scrollBox->addRow({answerLabel});
    

    if(!wasCorrect){
        std::string correction;
        correction= "x = " + std::to_string(static_cast<int>(correctAnswer[0]));
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        scrollBox->addRow({ correctionLabel });
    }
}

void PolinomialG1::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result, ec;
    SDL_Color color;
    ec = std::to_string(parameters[0]) + " * x " + " + " + std::to_string(parameters[1]) + " = " + std::to_string(parameters[2]);
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }

    ecLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, ec, 0, 0);
    
    pageBox->addRow({ ecLabel });

    result = "x = " + std::to_string(static_cast<int>(givenAnswer[0]));
    answerLabel = new MathLabel(FontSet.large, color, renderer, result, 0, 0);

    pageBox->addRow({answerLabel});
    

    if(!wasCorrect){
        std::string correction;
        correction= "x = " + std::to_string(static_cast<int>(correctAnswer[0]));
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        pageBox->addRow({ correctionLabel });
    }
}