#include "LimitInfPol.h"

LimitInfPol::LimitInfPol(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(4);
    correctAnswer.resize(1);
    generateProblem();
    calculateAnswer();
}

LimitInfPol::~LimitInfPol(){
    delete(hintLabel);
    delete(hintLabel2);
    delete(hintLabel3);
    delete(hintLabel4);
    delete(hintLim);
    delete(hintLim2);
    delete(hintFraction1);
    delete(hintFraction2);
    delete(ecuationLim);
    delete(ecuationFraction);
    delete(summaryLim);
    delete(summaryFraction);
    delete(summaryAnswer);
    delete(correctionLim);
    delete(correctionFraction);
    delete(correctionAnswer);
}

void LimitInfPol::calculateAnswer() {
    correctAnswer[0] = static_cast<double>(parameters[0]) / parameters[2];
}

void LimitInfPol::generateProblem() {
    std::uniform_int_distribution<int> dist(1, 10);
    correctAnswer[0] = dist(engine);
    parameters[2] = dist(engine);
    parameters[0] = static_cast<int>(correctAnswer[0]) * parameters[2];
    std::uniform_int_distribution<int> dist2(2, 99);
    parameters[1] = dist2(engine);
    parameters[3] = dist2(engine);
}

void LimitInfPol::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    ecuationLim = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLim->update(renderer, "lim", "x->inf", "", centerX-100, centerY);
    ecuationLim->drawLine = false;
    ecuationFraction = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    ecuationFraction->update(renderer, std::to_string(parameters[0]) + " * x + " + std::to_string(parameters[1]),
                                       std::to_string(parameters[2]) + " * x + " + std::to_string(parameters[3]), 
                                        " = ", centerX, centerY);

    hintLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    hintLabel->updateLeft(renderer, "L'Hopital's rule: ", hintX, hintY);
    
    hintLabel2 = new MathLabel(FontSet.medium, {0, 0, 0, 255});
    hintLabel2->updateLeft(renderer, "If the indeterminate case is ", hintX, hintY+50);

    hintLabel3 = new FractionLabel(FontSet.medium, {0, 0, 0, 255});
    hintLabel3->update(renderer, "inf", "inf", " or ", hintX, hintY+100);
    hintLabel3->reposition(hintX + 20, hintY+100);
    hintLabel4 = new FractionLabel(FontSet.medium, {0, 0, 0, 255});
    hintLabel4->update(renderer, "0", "0", ", then", hintX+90, hintY+100);
    
    hintLim = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    hintLim->drawLine = false;
    hintLim->update(renderer, "lim", "x->inf", "", hintX, hintY+170);
    hintLim->reposition(hintX + hintLim->rect.w/2, hintY+170);
    hintFraction1 = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    hintFraction1->update(renderer, "f(x)", "g(x)", " = ", hintX+100, hintY+170);
    hintLim2 = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    hintLim2->drawLine = false;
    hintLim2->update(renderer, "lim", "x->inf", "", hintX+200, hintY+170);
    hintFraction2 = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    hintFraction2->update(renderer, "f'(x)", "g'(x)", "", hintX+270, hintY+170);
}

std::vector<AnswerSlot> LimitInfPol::getAnswerSlots() {
    return { {centerX+100, centerY-25, 60, 50, ""} }; // one box
}

void LimitInfPol::drawExtra(SDL_Renderer* renderer) {
    if(ecuationFraction) ecuationFraction->draw(renderer);
    if(ecuationLim) ecuationLim->draw(renderer);
    
    if(hintChecked){
        if(hintLabel) hintLabel->draw(renderer);
        if(hintLabel2) hintLabel2->draw(renderer);
        if(hintLabel3) hintLabel3->draw(renderer);
        if(hintLabel4) hintLabel4->draw(renderer);
        if(hintLim) hintLim->draw(renderer);
        if(hintFraction1) hintFraction1->draw(renderer);
        if(hintLim2) hintLim2->draw(renderer);
        if(hintFraction2) hintFraction2->draw(renderer);
    }
}

void LimitInfPol::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }
    summaryLim = new FractionLabel(FontSet.large, color);
    summaryLim->update(renderer, "lim", "x->inf", "", 0, 0);
    summaryLim->drawLine = false;
    summaryFraction = new FractionLabel(FontSet.large, color);
    summaryFraction->update(renderer, std::to_string(parameters[0]) + " * x + " + std::to_string(parameters[1]),
                                       std::to_string(parameters[2]) + " * x + " + std::to_string(parameters[3]), 
                                        " = ", 0, 0);
    summaryAnswer = new MathLabel(FontSet.large, color);
    summaryAnswer->update(renderer, std::to_string(static_cast<int>(givenAnswer[0])), 0, 0);
    scrollBox->addRow({summaryLim, summaryFraction, summaryAnswer});

    if(!wasCorrect){
        correctionLim = new FractionLabel(FontSet.large, color);
        correctionLim->update(renderer, "lim", "x->inf", "", 0, 0);
        correctionLim->drawLine = false;
        correctionFraction = new FractionLabel(FontSet.large, color);
        correctionFraction->update(renderer, std::to_string(parameters[0]) + " * x + " + std::to_string(parameters[1]),
                                        std::to_string(parameters[2]) + " * x + " + std::to_string(parameters[3]), 
                                            " = ", 0, 0);
        correctionAnswer = new MathLabel(FontSet.large, color);
        correctionAnswer->update(renderer, std::to_string(static_cast<int>(correctAnswer[0])), 0, 0);
        scrollBox->addRow({correctionLim, correctionFraction, correctionAnswer});
    }
}

void LimitInfPol::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
    std::string result;
    SDL_Color color;
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }
    summaryLim = new FractionLabel(FontSet.large, color);
    summaryLim->update(renderer, "lim", "x->inf", "", 0, 0);
    summaryLim->drawLine = false;
    summaryFraction = new FractionLabel(FontSet.large, color);
    summaryFraction->update(renderer, std::to_string(parameters[0]) + " * x + " + std::to_string(parameters[1]),
                                       std::to_string(parameters[2]) + " * x + " + std::to_string(parameters[3]), 
                                        " = ", 0, 0);
    summaryAnswer = new MathLabel(FontSet.large, color);
    summaryAnswer->update(renderer, std::to_string(static_cast<int>(givenAnswer[0])), 0, 0);
    pageBox->addRow({summaryLim, summaryFraction, summaryAnswer});

    if(!wasCorrect){
        correctionLim = new FractionLabel(FontSet.large, {0, 0, 0, 255});
        correctionLim->update(renderer, "lim", "x->inf", "", 0, 0);
        correctionLim->drawLine = false;
        correctionFraction = new FractionLabel(FontSet.large, {0, 0, 0, 255});
        correctionFraction->update(renderer, std::to_string(parameters[0]) + " * x + " + std::to_string(parameters[1]),
                                        std::to_string(parameters[2]) + " * x + " + std::to_string(parameters[3]), 
                                            " = ", 0, 0);
        correctionAnswer = new MathLabel(FontSet.large, {0, 0, 0, 255});
        correctionAnswer->update(renderer, std::to_string(static_cast<int>(correctAnswer[0])), 0, 0);
        pageBox->addRow({correctionLim, correctionFraction, correctionAnswer});
    }
}