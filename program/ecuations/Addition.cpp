#include "Addition.h"

Addition::Addition(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(2);
    correctAnswer.resize(1);
    generateProblem();
    calculateAnswer();
}

Addition::~Addition(){
    delete(hintLabel);
    delete(ecuationLabel);
    delete(summaryLabel);
    delete(correctionLabel);
}

void Addition::calculateAnswer() {
    correctAnswer[0] = static_cast<double>(parameters[0]) + parameters[1];
}

void Addition::generateProblem() {
    std::uniform_int_distribution<int> dist(10, 100);
    parameters[0] = dist(engine); 
    parameters[1] = dist(engine);
}

void Addition::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    std::string text = std::to_string(parameters[0]) + " + " + std::to_string(parameters[1]) + " =";
    ecuationLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLabel->update(renderer, text, centerX-10, centerY);

    hintLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});    std::string hintText;
    int aux;
    if(parameters[0] % 10 != 0){
        aux = parameters[0] % 10;
        hintText = "Try " + std::to_string(parameters[0] - aux) + " + " + std::to_string(parameters[1] + aux);
    }
    else if(parameters[1] % 10 != 0){
        aux = parameters[1] % 10;
        hintText = "Try " + std::to_string(parameters[0] + aux) + " + " + std::to_string(parameters[1] - aux);
    }
    else{
        hintText = "Try " + std::to_string(parameters[0]/10) + " + " + std::to_string(parameters[1]/10) + " then add two zeros.";
    }
    hintLabel->updateLeft(renderer, hintText, hintX, hintY);
}

std::vector<AnswerSlot> Addition::getAnswerSlots() {
    return { {centerX+50, centerY-25, 60, 50, ""} }; // one box
}

void Addition::drawExtra(SDL_Renderer* renderer) {
    if(ecuationLabel) ecuationLabel->draw(renderer);
    
    if(hintChecked){
        if(hintLabel) hintLabel->draw(renderer);
    }
}

void Addition::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    result = std::to_string(parameters[0]) + " + " + 
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
        correction= std::to_string(parameters[0]) + " + " + 
                std::to_string(parameters[1]) + " = " + 
                std::to_string((int)correctAnswer[0]);
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        scrollBox->addRow({ correctionLabel });
    }
}

void Addition::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    result = std::to_string(parameters[0]) + " + " + 
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
        correction= std::to_string(parameters[0]) + " + " + 
                std::to_string(parameters[1]) + " = " + 
                std::to_string((int)correctAnswer[0]);
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        pageBox->addRow({ correctionLabel });
    }
}