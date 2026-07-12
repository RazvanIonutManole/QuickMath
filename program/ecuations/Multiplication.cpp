#include "Multiplication.h"

Multiplication::Multiplication(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(2);
    correctAnswer.resize(1);
    generateProblem();
    calculateAnswer();
}

Multiplication::~Multiplication(){
    delete(hintLabel);
    delete(ecuationLabel);
    delete(summaryLabel);
    delete(correctionLabel);
}

void Multiplication::calculateAnswer() {
    correctAnswer[0] = static_cast<double>(parameters[0]) * parameters[1];
}

void Multiplication::generateProblem() {
    std::uniform_int_distribution<int> dist(2, 10);
    parameters[0] = dist(engine); 
    std::uniform_int_distribution<int> dist1(10, 100);
    parameters[1] = dist1(engine);
}

void Multiplication::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    std::string text = std::to_string(parameters[0]) + " * " + std::to_string(parameters[1]) + " =";
    ecuationLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLabel->update(renderer, text, centerX-10, centerY);

    hintLabel = new MathLabel(FontSet.medium, {0, 0, 0, 255});
    std::string hintText = "Something went wrong";
    if(parameters[0] == 2)
        hintText = "It is the same as " + std::to_string(parameters[1]) + " + " + std::to_string(parameters[1]);
    else if(parameters[0] == 3)
        hintText = "It is the same as " + std::to_string(parameters[1]) + " + " + std::to_string(parameters[1]) + " + " + std::to_string(parameters[1]);
    else 
        hintText = "It is the same as " + std::to_string(parameters[1]) + " + " + std::to_string(parameters[1]) + " + ... " + std::to_string(parameters[0]) + " times.";
    
    hintLabel->updateLeft(renderer, hintText, hintX, hintY);
}

std::vector<AnswerSlot> Multiplication::getAnswerSlots() {
    return { {centerX+50, centerY-25, 60, 50, ""} }; // one box
}

void Multiplication::drawExtra(SDL_Renderer* renderer) {
    if(ecuationLabel) ecuationLabel->draw(renderer);

    if(hintChecked){
        if(hintLabel) hintLabel->draw(renderer);
    }
}

void Multiplication::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    result = std::to_string(parameters[0]) + " * " + 
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
        correction= std::to_string(parameters[0]) + " * " + 
                std::to_string(parameters[1]) + " = " + 
                std::to_string((int)correctAnswer[0]);
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        scrollBox->addRow({ correctionLabel });
    }
}

void Multiplication::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    result = std::to_string(parameters[0]) + " * " + 
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
        correction= std::to_string(parameters[0]) + " * " + 
                std::to_string(parameters[1]) + " = " + 
                std::to_string((int)correctAnswer[0]);
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        pageBox->addRow({ correctionLabel });
    }
}