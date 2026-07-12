#include "DerivativePolExp.h"

DerivativePolExp::DerivativePolExp(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(4);
    correctAnswer.resize(1);
    generateProblem();
    calculateAnswer();
}

DerivativePolExp::~DerivativePolExp(){
    delete(hintLabel1);
    delete(hintLabel2);
    delete(hintPower1);
    delete(hintPower2);
    delete(ecuationLabel);
    delete(ecuationPower);
    delete(dfx);
    delete(derivativeLabel);
    delete(derivativePower);
    delete(summaryLabel);
    delete(correctionLabel);
}

void DerivativePolExp::calculateAnswer() {
    //(a*(b*x+c)^d)'
    //=[a*b*d]*(b*x+c)^(d-1)
    correctAnswer[0] = static_cast<double>(parameters[0]*parameters[1]*parameters[3]);
}

void DerivativePolExp::generateProblem() {
    std::uniform_int_distribution<int> dist(2, 9);
    parameters[0] = dist(engine); 
    parameters[1] = dist(engine);
    parameters[2] = dist(engine); 
    parameters[3] = dist(engine);
}

void DerivativePolExp::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    std::string text = "f(x)= " + std::to_string(parameters[0]) + "* (" + std::to_string(parameters[1]) + " * x + ";
    ecuationLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLabel->update(renderer, text, centerX-10, centerY);
    ecuationPower = new PowerLabel(FontSet.large, FontSet.largeSuper,{0, 0, 0, 255});
    ecuationPower->update(renderer, std::to_string(parameters[2]) + ")", std::to_string(parameters[3]), centerX+80, centerY);

    dfx = new MathLabel(FontSet.large, {0, 0, 0, 255});
    dfx->update(renderer, "f'(x) = ", centerX-60, centerY+100);
    derivativeLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    derivativeLabel->update(renderer, "* (" + std::to_string(parameters[1]) + " * x + ", 
                    centerX+105, centerY+100);
    derivativePower = new PowerLabel(FontSet.large, FontSet.largeSuper,{0, 0, 0, 255});
    derivativePower->update(renderer, std::to_string(parameters[2]) + ")", std::to_string(parameters[3]-1), 
                    centerX+170, centerY+100);

    hintLabel1 = new MathLabel(FontSet.large, {0, 0, 0, 255});    
    hintLabel1->updateLeft(renderer, "a*(b*x+", hintX, hintY);
    hintPower1 = new PowerLabel(FontSet.large, FontSet.largeSuper, {0, 0, 0, 255});
    hintPower1->updateLeft(renderer, "c)", "d", hintX+75, hintY);
    hintLabel2 = new MathLabel(FontSet.large, {0, 0, 0, 255});
    hintLabel2->updateLeft(renderer, ")' = a*b*d*(b*x+", hintX+110, hintY);
    hintPower2 = new PowerLabel(FontSet.large, FontSet.largeSuper, {0, 0, 0, 255});
    hintPower2->updateLeft(renderer, "c)", "d-1", hintX+265, hintY);
}

std::vector<AnswerSlot> DerivativePolExp::getAnswerSlots() {
    return { {centerX-20, centerY+75, 60, 50, ""} }; // one box
}

void DerivativePolExp::drawExtra(SDL_Renderer* renderer) {
    if(ecuationLabel) ecuationLabel->draw(renderer);
    if(ecuationPower) ecuationPower->draw(renderer);
    if(dfx) dfx->draw(renderer);
    if(derivativeLabel) derivativeLabel->draw(renderer);
    if(derivativePower) derivativePower->draw(renderer);
    
    if(hintChecked){
        if(hintLabel1) hintLabel1->draw(renderer);
        if(hintLabel2) hintLabel2->draw(renderer);
        if(hintPower1) hintPower1->draw(renderer);
        if(hintPower2) hintPower2->draw(renderer);
    }
}

void DerivativePolExp::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
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

    std::string text = "f(x)= " + std::to_string(parameters[0]) + "* (" + std::to_string(parameters[1]) + " * x + ";
    summaryLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    summaryLabel->update(renderer, text, centerX-10, centerY);
    summaryPower = new PowerLabel(FontSet.large, FontSet.largeSuper,{0, 0, 0, 255});
    summaryPower->update(renderer, std::to_string(parameters[2]) + ")", std::to_string(parameters[3]), centerX+80, centerY);
    
    scrollBox->addRow({summaryLabel, summaryPower});
    
}

void DerivativePolExp::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
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

    std::string text = "f(x)= " + std::to_string(parameters[0]) + "* (" + std::to_string(parameters[1]) + " * x + ";
    summaryLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    summaryLabel->update(renderer, text, centerX-10, centerY);
    summaryPower = new PowerLabel(FontSet.large, FontSet.largeSuper, {0, 0, 0, 255});
    summaryPower->update(renderer, std::to_string(parameters[2]) + ")", std::to_string(parameters[3]), centerX+80, centerY);
    
    pageBox->addRow({summaryLabel, summaryPower});

    text = "f'(x)= " + std::to_string(static_cast<int>(givenAnswer[0])) + "* (" + std::to_string(parameters[1]) + " * x + ";
    answerLabel = new MathLabel(FontSet.large, color);
    answerLabel->update(renderer, text, centerX-10, centerY);
    answerPower = new PowerLabel(FontSet.large, FontSet.largeSuper, color);
    answerPower->update(renderer, std::to_string(parameters[2]) + ")", std::to_string(parameters[3]-1), centerX+80, centerY);

    pageBox->addRow({answerLabel, answerPower});

    if(!wasCorrect){
        text = "f'(x)= " + std::to_string(static_cast<int>(correctAnswer[0])) + "* (" + std::to_string(parameters[1]) + " * x + ";
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
        correctionLabel->update(renderer, text, centerX-10, centerY);
        correctionPower = new PowerLabel(FontSet.large, FontSet.largeSuper, {0, 0, 0, 255});
        correctionPower->update(renderer, std::to_string(parameters[2]) + ")", std::to_string(parameters[3]-1), centerX+80, centerY);
        pageBox->addRow({correctionLabel, correctionPower});
    }
}