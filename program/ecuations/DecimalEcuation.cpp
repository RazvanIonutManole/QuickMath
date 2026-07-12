#include "DecimalEcuation.h"

DecimalEcuation::DecimalEcuation(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(4);
    correctAnswer.resize(1);
    generateProblem();
    calculateAnswer();
}

DecimalEcuation::~DecimalEcuation(){
    delete(hintLabel);
    delete(hintLabel2);
    delete(ecuationLabel);
    delete(summaryLabel);
    delete(correctionLabel);
}

void DecimalEcuation::calculateAnswer() {
    correctAnswer[0] = static_cast<double>(parameters[0]) + static_cast<double>(parameters[1] / 100.0) + static_cast<double>(parameters[2]) + static_cast<double>(parameters[3] / 100.0);
}

void DecimalEcuation::generateProblem() {
    std::uniform_int_distribution<int> dist1(-9, 9);
    parameters[0] = dist1(engine);
    parameters[2] = dist1(engine);
    std::uniform_int_distribution<int> dist2(1, 99);
    parameters[1] = dist2(engine);
    parameters[3] = dist2(engine);
}

void DecimalEcuation::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    std::string text = 
    std::to_string(parameters[0]) + "." + (parameters[1] < 10 ? "0" : "") + std::to_string(parameters[1])
    + (parameters[2] < 0 ? " - " : " + ") + 
    std::to_string(parameters[2] < 0 ? -parameters[2] : parameters[2]) + "." + (parameters[3] < 10 ? "0" : "") + std::to_string(parameters[3])
    + " = ";
    ecuationLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLabel->update(renderer, text, centerX, centerY);

    hintLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    hintLabel->updateLeft(renderer, "It is the same as:", hintX, hintY);
    hintLabel2 = new MathLabel(FontSet.large, {0, 0, 0, 255});
    hintLabel2->updateLeft(renderer, "(" + 
                            (parameters[0] == 0 ? "" : std::to_string(parameters[0])) + std::to_string(parameters[1])
                            + (parameters[2] < 0 ? " - " : " + ") + 
                            (parameters[2] == 0 ? "" : std::to_string(parameters[2] < 0 ? -parameters[2] : parameters[2])) + std::to_string(parameters[3])
                            + ") / 100", hintX, hintY+50);
}

void DecimalEcuation::drawExtra(SDL_Renderer* renderer){
    if(ecuationLabel) ecuationLabel->draw(renderer);
    
    if(hintChecked){
        if(hintLabel) hintLabel->draw(renderer);
        if(hintLabel2) hintLabel2->draw(renderer);
    }
}

std::vector<AnswerSlot> DecimalEcuation::getAnswerSlots() {
    return { {centerX+100, centerY-25, 60, 50, ""} }; // one box
}

void DecimalEcuation::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;

    std::string s = std::to_string(givenAnswer[0]);
    s.erase(s.find_last_not_of('0') + 1);
    if (s.back() == '.') s.pop_back();

    result = 
        std::to_string(parameters[0]) + "." + (parameters[1] < 10 ? "0" : "") + std::to_string(parameters[1])
        + (parameters[2] < 0 ? " - " : " + ") + 
        std::to_string(parameters[2] < 0 ? -parameters[2] : parameters[2]) + "." + (parameters[3] < 10 ? "0" : "") + std::to_string(parameters[3])
        + " = " + s;
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

        s = std::to_string(correctAnswer[0]);
        s.erase(s.find_last_not_of('0') + 1);
        if (s.back() == '.') s.pop_back();

        correction= 
            std::to_string(parameters[0]) + "." + (parameters[1] < 10 ? "0" : "") + std::to_string(parameters[1])
            + (parameters[2] < 0 ? " - " : " + ") + 
            std::to_string(parameters[2] < 0 ? -parameters[2] : parameters[2]) + "." + (parameters[3] < 10 ? "0" : "") + std::to_string(parameters[3])
            + " = " + s;
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        scrollBox->addRow({ correctionLabel });
    }
}

void DecimalEcuation::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;

    std::string s = std::to_string(givenAnswer[0]);
    s.erase(s.find_last_not_of('0') + 1);
    if (s.back() == '.') s.pop_back();

    result = 
        std::to_string(parameters[0]) + "." + (parameters[1] < 10 ? "0" : "") + std::to_string(parameters[1])
        + (parameters[2] < 0 ? " - " : " + ") + 
        std::to_string(parameters[2] < 0 ? -parameters[2] : parameters[2]) + "." + (parameters[3] < 10 ? "0" : "") + std::to_string(parameters[3])
        + " = " + s;
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

        s = std::to_string(correctAnswer[0]);
        s.erase(s.find_last_not_of('0') + 1);
        if (s.back() == '.') s.pop_back();

        correction= 
            std::to_string(parameters[0]) + "." + (parameters[1] < 10 ? "0" : "") + std::to_string(parameters[1])
            + (parameters[2] < 0 ? " - " : " + ") + 
            std::to_string(parameters[2] < 0 ? -parameters[2] : parameters[2]) + "." + (parameters[3] < 10 ? "0" : "") + std::to_string(parameters[3])
            + " = " + s;
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255}, renderer, correction, 0, 0);
        pageBox->addRow({ correctionLabel });
    }
}