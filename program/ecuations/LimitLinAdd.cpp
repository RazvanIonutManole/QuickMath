#include "LimitLinAdd.h"

LimitLinAdd::LimitLinAdd(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(3);
    correctAnswer.resize(1);
    generateProblem();
    calculateAnswer();
}

LimitLinAdd::~LimitLinAdd(){
    delete(hintLabel);
    delete(ecuationLabel);
    delete(ecuationLim);
    delete(summaryLabel);
    delete(summaryLim);
    delete(correctionLabel);
    delete(correctionLim);
}

void LimitLinAdd::calculateAnswer() {
    correctAnswer[0] = static_cast<double>(parameters[0] * parameters[2]) + parameters[1] * parameters[2];
}

void LimitLinAdd::generateProblem() {
    std::uniform_int_distribution<int> dist(2, 20);
    parameters[0] = dist(engine); 
    parameters[1] = dist(engine);
    std::uniform_int_distribution<int> dist2(1, 10);
    parameters[2] = dist2(engine);
}

void LimitLinAdd::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    ecuationLim = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLim->update(renderer, "lim", "x->" + std::to_string(parameters[2]), "", centerX-100, centerY);
    ecuationLim->drawLine = false;
    ecuationLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLabel->update(renderer, std::to_string(parameters[0]) + " * x + " + 
                                    std::to_string(parameters[1]) + " * x = ", centerX, centerY);

    hintLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    hintLabel->updateLeft(renderer, "a*x + b*x = (a + b) * x", hintX, hintY);
}

std::vector<AnswerSlot> LimitLinAdd::getAnswerSlots() {
    return { {centerX+100, centerY-25, 60, 50, ""} }; // one box
}

void LimitLinAdd::drawExtra(SDL_Renderer* renderer) {
    if(ecuationLabel) ecuationLabel->draw(renderer);
    if(ecuationLim) ecuationLim->draw(renderer);
    
    if(hintChecked){
        if(hintLabel) hintLabel->draw(renderer);
    }
}

void LimitLinAdd::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
}

void LimitLinAdd::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
    SDL_Color color;
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }

    summaryLim = new FractionLabel(FontSet.large, color);
    summaryLim->update(renderer, "lim", "x->" + std::to_string(parameters[2]), "", 0, 0);
    summaryLim->drawLine = false;
    summaryLabel = new MathLabel(FontSet.large, color);
    summaryLabel->update(renderer, std::to_string(parameters[0]) + " * x + " + 
                                   std::to_string(parameters[1]) + " * x = " + std::to_string(static_cast<int>(givenAnswer[0])), 0,0);

    pageBox->addRow({summaryLim, summaryLabel});

    if(!wasCorrect){
        correctionLim = new FractionLabel(FontSet.large, {0, 0, 0, 255});
        correctionLim->update(renderer, "lim", "x->" + std::to_string(parameters[2]), "", 0, 0);
        correctionLim->drawLine = false;
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
        correctionLabel->update(renderer, std::to_string(parameters[0]) + " * x + " + 
                                    std::to_string(parameters[1]) + " * x = " + std::to_string(static_cast<int>(correctAnswer[0])), 0,0);

        pageBox->addRow({correctionLim, correctionLabel});
    }

}