#include "Substraction.h"

Substraction::Substraction(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(2);
    correctAnswer.resize(1);
    generateProblem();
    calculateAnswer();
}

Substraction::~Substraction(){
    delete(hintLabel);
    delete(ecuationLabel);
    delete(summaryLabel);
    delete(correctionLabel);
}

void Substraction::calculateAnswer() {
    correctAnswer[0] = static_cast<double>(parameters[0]) - parameters[1];
}

void Substraction::generateProblem() {
    std::uniform_int_distribution<int> dist(10, 100);
    parameters[0] = dist(engine); 
    parameters[1] = dist(engine);
}

void Substraction::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    std::string text = std::to_string(parameters[0]) + " - " + std::to_string(parameters[1]) + " =";
    ecuationLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLabel->update(renderer, text, centerX-10, centerY);

    hintLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    std::string hintText = "It is the same as x + " + std::to_string(parameters[1]) + " = " + std::to_string(parameters[0]);
    hintLabel->updateLeft(renderer, hintText, hintX, hintY);
}

std::vector<AnswerSlot> Substraction::getAnswerSlots() {
    return { {centerX+50, centerY-25, 60, 50, ""} }; // one box
}

void Substraction::drawExtra(SDL_Renderer* renderer) {
    if(ecuationLabel) ecuationLabel->draw(renderer);

    if(hintChecked){
        if(hintLabel) hintLabel->draw(renderer);
    }
}

void Substraction::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    result = std::to_string(parameters[0]) + " - " + 
            std::to_string(parameters[1]) + " = " + 
            std::to_string((int)givenAnswer[0]);
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }

    summaryLabel = new MathLabel(FontSet.large, color, renderer, result, 0, 0);
    
    scrollBox->addRow({ summaryLabel });

    if(!wasCorrect){
        std::string correction;
        correction= std::to_string(parameters[0]) + " - " + 
                std::to_string(parameters[1]) + " = " + 
                std::to_string((int)correctAnswer[0]);
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        scrollBox->addRow({ correctionLabel });
    }
}

void Substraction::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    result = std::to_string(parameters[0]) + " - " + 
            std::to_string(parameters[1]) + " = " + 
            std::to_string((int)givenAnswer[0]);
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }

    summaryLabel = new MathLabel(FontSet.large, color, renderer, result, 0, 0);
    
    pageBox->addRow({ summaryLabel });

    if(!wasCorrect){
        std::string correction;
        correction= std::to_string(parameters[0]) + " - " + 
                std::to_string(parameters[1]) + " = " + 
                std::to_string((int)correctAnswer[0]);
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        pageBox->addRow({ correctionLabel });
    }
}