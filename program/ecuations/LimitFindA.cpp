#include "LimitFindA.h"

LimitFindA::LimitFindA(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(4);
    correctAnswer.resize(1);
    generateProblem();
    calculateAnswer();
}

LimitFindA::~LimitFindA(){
    delete(hintLabel);
    delete(hintLabel2);
    delete(hintLabel3);
    delete(ecuationLabel);
    delete(ecuationFunction);
    delete(ecuationQuestion);
    delete(answerLabel);
    delete(summaryLabel);
    delete(summaryFunction);
    delete(summaryAnswer);
    delete(summaryQuestion);
    delete(correctionLabel);
}

void LimitFindA::calculateAnswer() {
    //f(x) = p0*x+a  x<p3
    //f(x) = p1*x+p2 x>=p3
    //a=p1*p3+p2-p0*p3
    correctAnswer[0] = static_cast<double>(parameters[1] * parameters[3]) + parameters[2] - parameters[0] * parameters[3];
}

void LimitFindA::generateProblem() {
    std::uniform_int_distribution<int> dist(1, 10);
    parameters[0] = dist(engine);
    parameters[1] = dist(engine);
    parameters[2] = dist(engine);
    parameters[3] = dist(engine);
}

void LimitFindA::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    ecuationLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    ecuationLabel->update(renderer, "f(x) = ", centerX-70, centerY);
    ecuationFunction = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    ecuationFunction->drawLine=false;
    ecuationFunction->update(renderer, std::to_string(parameters[0]) + " * x + A, x > " + std::to_string(parameters[3]),
                                       std::to_string(parameters[1]) + " * x + " + std::to_string(parameters[2]) + ", x <= " + std::to_string(parameters[3]),
                                    "", centerX + 50, centerY);
    ecuationQuestion = new MathLabel(FontSet.large, {0, 0, 0, 255});
    ecuationQuestion->update(renderer, "Find A so that f has a limit in x = " + std::to_string(parameters[3]), centerX+50, centerY+75);

    answerLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    answerLabel->update(renderer, "A = ", centerX-30, centerY+150);

    hintLabel = new MathLabel(FontSet.medium, {0, 0, 0, 255});
    hintLabel->updateLeft(renderer, "For the limit to exist,", hintX, hintY);
    hintLabel2 = new MathLabel(FontSet.medium, {0, 0, 0, 255});
    hintLabel2->updateLeft(renderer, "f(x) needes to give the same value", hintX, hintY+50);
    hintLabel3 = new MathLabel(FontSet.medium, {0, 0, 0, 255});
    hintLabel3->updateLeft(renderer, "in both branches", hintX, hintY+100);
}

std::vector<AnswerSlot> LimitFindA::getAnswerSlots() {
    return { {centerX, centerY+125, 60, 50, ""} }; // one box
}

void LimitFindA::drawExtra(SDL_Renderer* renderer) {
    if(ecuationLabel) ecuationLabel->draw(renderer);
    if(ecuationFunction) ecuationFunction->draw(renderer);
    if(ecuationQuestion) ecuationQuestion->draw(renderer);
    if(answerLabel) answerLabel->draw(renderer);
    
    if(hintChecked){
        if(hintLabel) hintLabel->draw(renderer);
        if(hintLabel2) hintLabel2->draw(renderer);
        if(hintLabel3) hintLabel3->draw(renderer);
    }
}

void LimitFindA::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) {
    SDL_Color color;
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }

    summaryLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    summaryLabel->update(renderer, "f(x) = ", 0, 0);
    summaryFunction = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    summaryFunction->drawLine=false;
    summaryFunction->update(renderer, std::to_string(parameters[0]) + " * x + A, x > " + std::to_string(parameters[3]),
                                       std::to_string(parameters[1]) + " * x + " + std::to_string(parameters[2]) + ", x <= " + std::to_string(parameters[3]),
                                    "", 0, 0);

    scrollBox->addRow({summaryLabel, summaryFunction});

    summaryQuestion = new MathLabel(FontSet.medium, {0, 0, 0, 255});
    summaryQuestion->update(renderer, "Find A so that f has a limit in x = " + std::to_string(parameters[3]), 0, 0);
    
    scrollBox->addRow({summaryQuestion});

    summaryAnswer = new MathLabel(FontSet.large, color);
    summaryAnswer->update(renderer, "A = " + std::to_string(static_cast<int>(givenAnswer[0])), 0, 0);

    scrollBox->addRow({summaryAnswer});

    if(!wasCorrect){
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
        correctionLabel->update(renderer, "A = " + std::to_string(static_cast<int>(correctAnswer[0])), 0, 0);

        scrollBox->addRow({correctionLabel});
    }
}

void LimitFindA::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet FontSet) {
    SDL_Color color;
    if(wasCorrect){
        color = {0, 255, 0, 255};
    }
    else{
        color = {255, 0, 0, 255};
    }

    summaryLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
    summaryLabel->update(renderer, "f(x) = ", 0, 0);
    summaryFunction = new FractionLabel(FontSet.large, {0, 0, 0, 255});
    summaryFunction->drawLine=false;
    summaryFunction->update(renderer, std::to_string(parameters[0]) + " * x + A, x > " + std::to_string(parameters[3]),
                                       std::to_string(parameters[1]) + " * x + " + std::to_string(parameters[2]) + ", x <= " + std::to_string(parameters[3]),
                                    "", 0, 0);

    pageBox->addRow({summaryLabel, summaryFunction});

    summaryQuestion = new MathLabel(FontSet.medium, {0, 0, 0, 255});
    summaryQuestion->update(renderer, "Find A so that f has a limit in x = " + std::to_string(parameters[3]), 0, 0);
    
    pageBox->addRow({summaryQuestion});

    summaryAnswer = new MathLabel(FontSet.large, color);
    summaryAnswer->update(renderer, "A = " + std::to_string(static_cast<int>(givenAnswer[0])), 0, 0);

    pageBox->addRow({summaryAnswer});

    if(!wasCorrect){
        correctionLabel = new MathLabel(FontSet.large, {0, 0, 0, 255});
        correctionLabel->update(renderer, "A = " + std::to_string(static_cast<int>(correctAnswer[0])), 0, 0);

        pageBox->addRow({correctionLabel});
    }

}