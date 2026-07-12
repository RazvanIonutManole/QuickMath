#include "DerivativeTrig.h"
#include <iostream>

DerivativeTrig::DerivativeTrig(std::mt19937& engine) : Ecuation(engine) {
    parameters.resize(4);
    correctAnswer.resize(2);
    generateProblem();
    calculateAnswer();
}

DerivativeTrig::~DerivativeTrig() {
    delete fxLabel;
    delete dfxLabel;
    delete sinLabel;
    delete cosLabel;
    delete plusLabel;
    delete sinHintLabel;
    delete cosHintLabel;
    delete ecuationLabel;
    delete answerLabel;
    delete correctionLabel;
}

void DerivativeTrig::calculateAnswer() {
    // f(x) = a*sin(bx) + c*cos(dx)
    // f'(x) = [box1]sin(dx) + [box2]cos(bx)
    // box1 = -c*d (from differentiating cos)
    // box2 = a*b  (from differentiating sin)
    correctAnswer[0] = -static_cast<double>(parameters[1] * parameters[3]);
    correctAnswer[1] = static_cast<double>(parameters[0] * parameters[2]);
}

void DerivativeTrig::generateProblem() {
    std::uniform_int_distribution<int> orderDist(0, 1);
    order = orderDist(engine);

    std::uniform_int_distribution<int> coeffDist(1, 10);
    std::uniform_int_distribution<int> signDist(0, 1);

    parameters[0] = coeffDist(engine) * (signDist(engine) ? 1 : -1); // a
    parameters[1] = coeffDist(engine) * (signDist(engine) ? 1 : -1); // c
    parameters[2] = coeffDist(engine) * (signDist(engine) ? 1 : -1); // b
    parameters[3] = coeffDist(engine) * (signDist(engine) ? 1 : -1); // d
}

static std::string fmtCoeff(int val, bool absOnly) {
    int v = absOnly ? std::abs(val) : val;
    if (v == 1)  return "";
    if (v == -1) return "-";
    return std::to_string(v);
}

static std::string fmtArg(int val) {
    if (val == 1)  return "";
    if (val == -1) return "-";
    return std::to_string(val);
}

void DerivativeTrig::buildDisplay(SDL_Renderer* renderer, FontSet fontSet) {
    std::vector<std::string> param(4);
    // order==1: sin goes first, its coeff shows with sign, cos coeff is shown absolute
    // order==0: cos goes first, its coeff shows with sign, sin coeff is shown absolute
    param[0] = fmtCoeff(parameters[0], false); // always show sign
    param[1] = fmtCoeff(parameters[1], false); // always show sign
    param[2] = fmtArg(parameters[2]);               // sin arg
    param[3] = fmtArg(parameters[3]);               // cos arg

    std::string firstTerm = (order == 1)
        ? param[0] + "sin(" + param[2] + "x)"
        : param[1] + "cos(" + param[3] + "x)";
    std::string secondTerm = (order == 0)
        ? param[0] + "sin(" + param[2] + "x)"
        : param[1] + "cos(" + param[3] + "x)";

    std::string text = "f(x) = " + firstTerm + " + " + secondTerm;

    SDL_Color black = {0, 0, 0, 255};

    fxLabel = new MathLabel(fontSet.large, black);
    fxLabel->update(renderer, text, centerX+20, centerY);

    int dfxY = centerY + 100;

    dfxLabel = new MathLabel(fontSet.large, black);
    dfxLabel->update(renderer, "f'(x) =", centerX - 110, dfxY);

    // f'(x) is always: [box1]sin(dx) + [box2]cos(bx)
    cosLabel = new MathLabel(fontSet.large, black);
    cosLabel->update(renderer, "sin(" + param[3] + "x)", centerX +45, dfxY);

    plusLabel = new MathLabel(fontSet.large, black);
    plusLabel->update(renderer, "+", centerX + 55, dfxY);

    sinLabel = new MathLabel(fontSet.large, black);
    sinLabel->update(renderer, "cos(" + param[2] + "x)", centerX + 230, dfxY);

    sinHintLabel = new MathLabel(fontSet.large, black);
    sinHintLabel->updateLeft(renderer, "sin'(a*x) = a*cos(x)", hintX, hintY);

    cosHintLabel = new MathLabel(fontSet.large, black);
    cosHintLabel->updateLeft(renderer, "cos'(a*x) = -a*sin(x)", hintX, hintY + 30);
}

void DerivativeTrig::drawExtra(SDL_Renderer* renderer) {
    if (fxLabel)   fxLabel->draw(renderer);
    if (dfxLabel)  dfxLabel->draw(renderer);
    if (cosLabel)  cosLabel->draw(renderer);
    if (plusLabel) plusLabel->draw(renderer);
    if (sinLabel)  sinLabel->draw(renderer);

    if (hintChecked) {
        if (sinHintLabel) sinHintLabel->draw(renderer);
        if (cosHintLabel) cosHintLabel->draw(renderer);
    }
}

std::vector<AnswerSlot> DerivativeTrig::getAnswerSlots() {
    return {
        {centerX - 70, centerY + 75, 60, 50, ""},
        {centerX + 120,  centerY + 75, 60, 50, ""}
    };
}

void DerivativeTrig::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet fontSet) {
    std::vector<std::string> param(4);
    param[0] = fmtCoeff(parameters[0], false); // always show sign
    param[1] = fmtCoeff(parameters[1], false); // always show sign
    param[2] = fmtArg(parameters[2]);
    param[3] = fmtArg(parameters[3]);

    std::string firstTerm = (order == 1)
        ? param[0] + "sin(" + param[2] + "x)"
        : param[1] + "cos(" + param[3] + "x)";
    std::string secondTerm = (order == 0)
        ? param[0] + "sin(" + param[2] + "x)"
        : param[1] + "cos(" + param[3] + "x)";

    std::string equationText = "f(x) = " + firstTerm + " + " + secondTerm;

    ecuationLabel = new MathLabel(fontSet.medium, {0, 0, 0, 255}, renderer, equationText, 0, 0);
    scrollBox->addRow({ecuationLabel});

    SDL_Color color = wasCorrect ? SDL_Color{0, 200, 0, 255} : SDL_Color{220, 0, 0, 255};

    std::string ans0 = std::to_string(static_cast<int>(givenAnswer[0]));
    std::string ans1 = std::to_string(static_cast<int>(givenAnswer[1]));
    std::string answerText = "f'(x) = " + ans0 + "sin(" + param[3] + "x) + " +
                              ans1 + "cos(" + param[2] + "x)";

    answerLabel = new MathLabel(fontSet.medium, color, renderer, answerText, 0, 0);
    scrollBox->addRow({answerLabel});

    if (!wasCorrect) {
        std::string cor0 = std::to_string(static_cast<int>(correctAnswer[0]));
        std::string cor1 = std::to_string(static_cast<int>(correctAnswer[1]));
        std::string correctionText = "f'(x) = " + cor0 + "sin(" + param[3] + "x) + " +
                                      cor1 + "cos(" + param[2] + "x)";

        correctionLabel = new MathLabel(fontSet.medium, {0, 0, 0, 255}, renderer, correctionText, 0, 0);
        scrollBox->addRow({correctionLabel});
    }
}


void DerivativeTrig::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet fontSet) {
    std::vector<std::string> param(4);
    param[0] = fmtCoeff(parameters[0], false); // always show sign
    param[1] = fmtCoeff(parameters[1], false); // always show sign
    param[2] = fmtArg(parameters[2]);
    param[3] = fmtArg(parameters[3]);

    std::string firstTerm = (order == 1)
        ? param[0] + "sin(" + param[2] + "x)"
        : param[1] + "cos(" + param[3] + "x)";
    std::string secondTerm = (order == 0)
        ? param[0] + "sin(" + param[2] + "x)"
        : param[1] + "cos(" + param[3] + "x)";

    std::string equationText = "f(x) = " + firstTerm + " + " + secondTerm;

    ecuationLabel = new MathLabel(fontSet.medium, {0, 0, 0, 255}, renderer, equationText, 0, 0);
    pageBox->addRow({ecuationLabel});

    SDL_Color color = wasCorrect ? SDL_Color{0, 200, 0, 255} : SDL_Color{220, 0, 0, 255};

    std::string ans0 = std::to_string(static_cast<int>(givenAnswer[0]));
    std::string ans1 = std::to_string(static_cast<int>(givenAnswer[1]));
    std::string answerText = "f'(x) = " + ans0 + "sin(" + param[3] + "x) + " +
                              ans1 + "cos(" + param[2] + "x)";

    answerLabel = new MathLabel(fontSet.medium, color, renderer, answerText, 0, 0);
    pageBox->addRow({answerLabel});

    if (!wasCorrect) {
        std::string cor0 = std::to_string(static_cast<int>(correctAnswer[0]));
        std::string cor1 = std::to_string(static_cast<int>(correctAnswer[1]));
        std::string correctionText = "f'(x) = " + cor0 + "sin(" + param[3] + "x) + " +
                                      cor1 + "cos(" + param[2] + "x)";

        correctionLabel = new MathLabel(fontSet.medium, {0, 0, 0, 255}, renderer, correctionText, 0, 0);
        pageBox->addRow({correctionLabel});
    }
}