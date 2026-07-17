#include "DerivativePol.h"

DerivativePol::DerivativePol(std::mt19937& engine) : Ecuation(engine) {
    std::uniform_int_distribution<int> dist(1, 3);
    grade = dist(engine);
    parameters.resize(grade + 1);
    correctAnswer.resize(grade);
    centerX += -(grade - 2) * 20 - (grade - 1) * 10;
    generateProblem();
    calculateAnswer();
}

DerivativePol::~DerivativePol() {
    delete fxLabel;
    delete fxLinearLabel;
    delete fxConstantLabel;
    delete dfxLabel;
    delete dfxLinearLabel;
    delete dfxConstantLabel;
    delete hint1;
    delete XToB;
    delete hint2;
    delete XToB2;
    for (auto* p : fxPowerLabels)    delete p;
    for (auto* p : dfxPowerLabels)   delete p;
    for (auto* p : fxOperatorLabels)  delete p;
    for (auto* p : dfxOperatorLabels) delete p;
    for (auto* l : summaryLabels) delete l;
}

void DerivativePol::generateProblem() {
    std::uniform_int_distribution<int> dist(1, 10);
    for (int i = 0; i < (int)parameters.size(); i++)
        parameters[i] = dist(engine);
}

void DerivativePol::calculateAnswer() {
    // f(x)  = p[0]*x^grade + p[1]*x^(grade-1) + ... + p[grade]
    // f'(x) = p[0]*grade * x^(grade-1) + p[1]*(grade-1)*x^(grade-2) + ...
    // correctAnswer[i] = coefficient of the i-th term of the derivative
    for (int i = 0; i < grade; i++)
        correctAnswer[i] = static_cast<double>(parameters[i]) * (grade - i);
}

// Helper: builds one row of term labels
// terms: coefficients from highest to lowest power, starting at startExp
// returns the x position after the last term
static int buildRow(
    SDL_Renderer* renderer, TTF_Font* font, TTF_Font* superFont,
    SDL_Color color,
    const std::vector<int>& coeffs, int startExp,
    std::vector<PowerLabel*>& outPower,
    std::vector<MathLabel*>& outOps,
    MathLabel*& outLinear,
    MathLabel*& outConstant,
    int startX, int y,
    bool useBoxPlaceholder,   // true for f'(x) row
    const std::vector<int>& answerCoeffs  // derivative coefficients (for placeholder widths)
) {
    int x = startX;
    int termCount = (int)coeffs.size();

    for (int i = 0; i < termCount; i++) {
        int exp = startExp - i;
        std::string coeff = std::to_string(coeffs[i]);

        if (exp >= 2) {
            auto* pl = new PowerLabel(font, superFont, color);
            if (useBoxPlaceholder) {
                pl->update(renderer, "x", (exp >2 ? std::to_string(exp-1) : ""), x, y);
            } else {
                pl->update(renderer, coeff + "x", std::to_string(exp), x, y);
            }
            x += 35;
            outPower.push_back(pl);
        } else if (exp == 1) {
            outLinear = new MathLabel(font, color);
            if (useBoxPlaceholder) {
            } else {
                outLinear->update(renderer, coeff + "x", x, y);
            }
            x += 35;
        } else { // constant
            outConstant = new MathLabel(font, color);
            if (useBoxPlaceholder) {
            } else {
                outConstant->update(renderer, coeff, x, y);
            }
            x += 35;
        }

        if (i < termCount - 1) {
            if(useBoxPlaceholder && exp == 1) break;
            auto* op = new MathLabel(font, color);
            op->update(renderer, "+", x, y);
            outOps.push_back(op);
            if(useBoxPlaceholder){
                x += 80;
            }
            else{
                x += 25;
            }
        }
    }
    return x;
}

void DerivativePol::buildDisplay(SDL_Renderer* renderer, FontSet FontSet) {
    if (labelsBuilt) return;
    labelsBuilt = true;

    SDL_Color black = {0, 0, 0, 255};
    int fxY  = centerY;  // f(x)  row
    int dfxY = centerY + 100;  // f'(x) row

    // --- f(x) row ---
    fxLabel = new MathLabel(FontSet.large, black);
    fxLabel->update(renderer, "f(x) =", centerX - 70, fxY);

    buildRow(renderer, FontSet.large, FontSet.largeSuper, black,
             parameters, grade,
             fxPowerLabels, fxOperatorLabels,
             fxLinearLabel, fxConstantLabel,
             centerX, fxY,
             false, {});

    // --- f'(x) row ---
    dfxLabel = new MathLabel(FontSet.large, black);
    dfxLabel->update(renderer, "f'(x) =", centerX - 70, dfxY);

    // derivative coefficients for display
    std::vector<int> dCoeffs;
    for (int i = 0; i < grade; i++)
        dCoeffs.push_back(parameters[i] * (grade - i));

    buildRow(renderer, FontSet.large, FontSet.largeSuper, black,
             parameters, grade,   // same count, exp shifts down by 1
             dfxPowerLabels, dfxOperatorLabels,
             dfxLinearLabel, dfxConstantLabel,
             centerX + 40, dfxY,
             true, dCoeffs);

    //a*x^b = a*b*x^(b-1)
    std::string textHint1 = "(a*";
    hint1 = new MathLabel(FontSet.large, black);
    hint1->updateLeft(renderer, textHint1, hintX, hintY);

    XToB = new PowerLabel(FontSet.large, FontSet.largeSuper, black);
    XToB->updateLeft(renderer, "x", "b", hintX+30, hintY);

    std::string textHint2 = ")' = a * b";
    hint2 = new MathLabel(FontSet.large, black);
    hint2->updateLeft(renderer, textHint2, hintX+60, hintY);

    XToB2 = new PowerLabel(FontSet.large, FontSet.largeSuper, black);
    XToB2->updateLeft(renderer, "x", "b - 1", hintX+155, hintY);
}

void DerivativePol::drawExtra(SDL_Renderer* renderer) {
    if (fxLabel)       fxLabel->draw(renderer);
    for (auto* p : fxPowerLabels)   p->draw(renderer);
    for (auto* p : fxOperatorLabels) p->draw(renderer);
    if (fxLinearLabel)   fxLinearLabel->draw(renderer);
    if (fxConstantLabel) fxConstantLabel->draw(renderer);

    if (dfxLabel)      dfxLabel->draw(renderer);
    for (auto* p : dfxPowerLabels)   p->draw(renderer);
    for (auto* p : dfxOperatorLabels) p->draw(renderer);
    if (dfxLinearLabel)   dfxLinearLabel->draw(renderer);
    if (dfxConstantLabel) dfxConstantLabel->draw(renderer);

    if(hintChecked){
        if(hint1) hint1->draw(renderer);
        if(XToB) XToB->draw(renderer);
        if(hint2) hint2->draw(renderer);
        if(XToB2) XToB2->draw(renderer);
    }
}

std::vector<AnswerSlot> DerivativePol::getAnswerSlots() {
    // one box per derivative term, laid out on the f'(x) row
    std::vector<AnswerSlot> slots;
    int x = centerX - 30;
    int y = centerY + 100;

    for (int i = 0; i < grade; i++) {
        int exp = grade - 1 - i;

        slots.push_back({x, y - 25, 50, 40, ""});

        if (exp >= 2)      x += 95 + 25;
        else if (exp == 1) x += 90 + 25;
        else               x += 85 + 25;
    }
    return slots;
}

void DerivativePol::addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet fontSet) {
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color color = wasCorrect ? SDL_Color{0, 200, 0, 255} : SDL_Color{220, 0, 0, 255};

    // --- f(x) row ---
    if (grade == 1) {
    std::string ansText = "f'(x) = " + std::to_string(static_cast<int>(parameters[0])) + "x + " + std::to_string(static_cast<int>(parameters[1]));
        auto* ml = new MathLabel(fontSet.large, color, renderer, ansText, 0, 0);
        summaryLabels.push_back(ml);
        scrollBox->addRow({ml});
    } else {
        std::vector<Label*> fxRow;
        auto* fxPrefix = new MathLabel(fontSet.medium, black, renderer, "f(x) =", 0, 0);
        summaryLabels.push_back(fxPrefix);
        fxRow.push_back(fxPrefix);

        for (int i = 0; i <= grade; i++) {
            int exp = grade - i;
            std::string coeff = std::to_string(parameters[i]);

            Label* termLabel;
            if (exp >= 2) {
                auto* pl = new PowerLabel(fontSet.medium, fontSet.mediumSuper, black,
                                        renderer, coeff + "x", std::to_string(exp), 0, 0);
                summaryLabels.push_back(pl);
                termLabel = pl;
            } else if (exp == 1) {
                auto* ml = new MathLabel(fontSet.medium, black, renderer, coeff + "x", 0, 0);
                summaryLabels.push_back(ml);
                termLabel = ml;
            } else {
                auto* ml = new MathLabel(fontSet.medium, black, renderer, coeff, 0, 0);
                summaryLabels.push_back(ml);
                termLabel = ml;
            }
            fxRow.push_back(termLabel);

            if (i < grade) {
                auto* op = new MathLabel(fontSet.medium, black, renderer, "+", 0, 0);
                summaryLabels.push_back(op);
                fxRow.push_back(op);
            }
        }
        scrollBox->addRow(fxRow);
    }

    // --- f'(x) answer row ---
    if (grade == 1) {
        std::string ansText = "f'(x) = " + std::to_string(static_cast<int>(correctAnswer[0]));
        auto* ml = new MathLabel(fontSet.large, color, renderer, ansText, 0, 0);
        summaryLabels.push_back(ml);
        scrollBox->addRow({ml});
    } else {
        std::vector<Label*> ansRow;
        auto* ansPrefix = new MathLabel(fontSet.medium, color, renderer, "f'(x) =", 0, 0);
        summaryLabels.push_back(ansPrefix);
        ansRow.push_back(ansPrefix);

        for (int i = 0; i < grade; i++) {
            int exp = grade - 1 - i;
            std::string ans = std::to_string(static_cast<int>(givenAnswer[i]));

            Label* termLabel;
            if (exp >= 2) {
                auto* pl = new PowerLabel(fontSet.medium, fontSet.mediumSuper, color,
                                        renderer, ans + "x", std::to_string(exp), 0, 0);
                summaryLabels.push_back(pl);
                termLabel = pl;
            } else if (exp == 1) {
                auto* ml = new MathLabel(fontSet.medium, color, renderer, ans + "x", 0, 0);
                summaryLabels.push_back(ml);
                termLabel = ml;
            } else {
                auto* ml = new MathLabel(fontSet.medium, color, renderer, ans, 0, 0);
                summaryLabels.push_back(ml);
                termLabel = ml;
            }
            ansRow.push_back(termLabel);

            if (i < grade - 1) {
                auto* op = new MathLabel(fontSet.medium, color, renderer, "+", 0, 0);
                summaryLabels.push_back(op);
                ansRow.push_back(op);
            }
        }
        scrollBox->addRow(ansRow);
    }

    // --- correction row ---
    if (!wasCorrect) {
        if (grade == 1) {
            std::string ansText = "f'(x) = " + std::to_string(static_cast<int>(givenAnswer[0]));
            auto* ml = new MathLabel(fontSet.large, color, renderer, ansText, 0, 0);
            summaryLabels.push_back(ml);
            scrollBox->addRow({ml});
        } else {
            std::vector<Label*> corrRow;
            auto* corrPrefix = new MathLabel(fontSet.medium, black, renderer, "f'(x) =", 0, 0);
            summaryLabels.push_back(corrPrefix);
            corrRow.push_back(corrPrefix);

            for (int i = 0; i < grade; i++) {
                int exp = grade - 1 - i;
                std::string cor = std::to_string(static_cast<int>(correctAnswer[i]));

                Label* termLabel;
                if (exp >= 2) {
                    auto* pl = new PowerLabel(fontSet.medium, fontSet.mediumSuper, black,
                                            renderer, cor + "x", std::to_string(exp), 0, 0);
                    summaryLabels.push_back(pl);
                    termLabel = pl;
                } else if (exp == 1) {
                    auto* ml = new MathLabel(fontSet.medium, black, renderer, cor + "x", 0, 0);
                    summaryLabels.push_back(ml);
                    termLabel = ml;
                } else {
                    auto* ml = new MathLabel(fontSet.medium, black, renderer, cor, 0, 0);
                    summaryLabels.push_back(ml);
                    termLabel = ml;
                }
                corrRow.push_back(termLabel);

                if (i < grade - 1) {
                    auto* op = new MathLabel(fontSet.medium, black, renderer, "+", 0, 0);
                    summaryLabels.push_back(op);
                    corrRow.push_back(op);
                }
            }
            scrollBox->addRow(corrRow);
        }
    }
}

void DerivativePol::addToPageBox(PageBox* pageBox, SDL_Renderer* renderer, FontSet fontSet) {
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color color = wasCorrect ? SDL_Color{0, 200, 0, 255} : SDL_Color{220, 0, 0, 255};

    // --- f(x) row ---
    if (grade == 1) {
    std::string ansText = "f'(x) = " + std::to_string(static_cast<int>(parameters[0])) + "x + " + std::to_string(static_cast<int>(parameters[1]));
        auto* ml = new MathLabel(fontSet.large, color, renderer, ansText, 0, 0);
        summaryLabels.push_back(ml);
        pageBox->addRow({ml});
    } else {
        std::vector<Label*> fxRow;
        auto* fxPrefix = new MathLabel(fontSet.medium, black, renderer, "f(x) =", 0, 0);
        summaryLabels.push_back(fxPrefix);
        fxRow.push_back(fxPrefix);

        for (int i = 0; i <= grade; i++) {
            int exp = grade - i;
            std::string coeff = std::to_string(parameters[i]);

            Label* termLabel;
            if (exp >= 2) {
                auto* pl = new PowerLabel(fontSet.medium, fontSet.mediumSuper, black,
                                        renderer, coeff + "x", std::to_string(exp), 0, 0);
                summaryLabels.push_back(pl);
                termLabel = pl;
            } else if (exp == 1) {
                auto* ml = new MathLabel(fontSet.medium, black, renderer, coeff + "x", 0, 0);
                summaryLabels.push_back(ml);
                termLabel = ml;
            } else {
                auto* ml = new MathLabel(fontSet.medium, black, renderer, coeff, 0, 0);
                summaryLabels.push_back(ml);
                termLabel = ml;
            }
            fxRow.push_back(termLabel);

            if (i < grade) {
                auto* op = new MathLabel(fontSet.medium, black, renderer, "+", 0, 0);
                summaryLabels.push_back(op);
                fxRow.push_back(op);
            }
        }
        pageBox->addRow(fxRow);
    }

    // --- f'(x) answer row ---
    if (grade == 1) {
        std::string ansText = "f'(x) = " + std::to_string(static_cast<int>(correctAnswer[0]));
        auto* ml = new MathLabel(fontSet.large, color, renderer, ansText, 0, 0);
        summaryLabels.push_back(ml);
        pageBox->addRow({ml});
    } else {
        std::vector<Label*> ansRow;
        auto* ansPrefix = new MathLabel(fontSet.medium, color, renderer, "f'(x) =", 0, 0);
        summaryLabels.push_back(ansPrefix);
        ansRow.push_back(ansPrefix);

        for (int i = 0; i < grade; i++) {
            int exp = grade - 1 - i;
            std::string ans = std::to_string(static_cast<int>(givenAnswer[i]));

            Label* termLabel;
            if (exp >= 2) {
                auto* pl = new PowerLabel(fontSet.medium, fontSet.mediumSuper, color,
                                        renderer, ans + "x", std::to_string(exp), 0, 0);
                summaryLabels.push_back(pl);
                termLabel = pl;
            } else if (exp == 1) {
                auto* ml = new MathLabel(fontSet.medium, color, renderer, ans + "x", 0, 0);
                summaryLabels.push_back(ml);
                termLabel = ml;
            } else {
                auto* ml = new MathLabel(fontSet.medium, color, renderer, ans, 0, 0);
                summaryLabels.push_back(ml);
                termLabel = ml;
            }
            ansRow.push_back(termLabel);

            if (i < grade - 1) {
                auto* op = new MathLabel(fontSet.medium, color, renderer, "+", 0, 0);
                summaryLabels.push_back(op);
                ansRow.push_back(op);
            }
        }
        pageBox->addRow(ansRow);
    }

    // --- correction row ---
    if (!wasCorrect) {
        if (grade == 1) {
            std::string ansText = "f'(x) = " + std::to_string(static_cast<int>(givenAnswer[0]));
            auto* ml = new MathLabel(fontSet.large, color, renderer, ansText, 0, 0);
            summaryLabels.push_back(ml);
            pageBox->addRow({ml});
        } else {
            std::vector<Label*> corrRow;
            auto* corrPrefix = new MathLabel(fontSet.medium, black, renderer, "f'(x) =", 0, 0);
            summaryLabels.push_back(corrPrefix);
            corrRow.push_back(corrPrefix);

            for (int i = 0; i < grade; i++) {
                int exp = grade - 1 - i;
                std::string cor = std::to_string(static_cast<int>(correctAnswer[i]));

                Label* termLabel;
                if (exp >= 2) {
                    auto* pl = new PowerLabel(fontSet.medium, fontSet.mediumSuper, black,
                                            renderer, cor + "x", std::to_string(exp), 0, 0);
                    summaryLabels.push_back(pl);
                    termLabel = pl;
                } else if (exp == 1) {
                    auto* ml = new MathLabel(fontSet.medium, black, renderer, cor + "x", 0, 0);
                    summaryLabels.push_back(ml);
                    termLabel = ml;
                } else {
                    auto* ml = new MathLabel(fontSet.medium, black, renderer, cor, 0, 0);
                    summaryLabels.push_back(ml);
                    termLabel = ml;
                }
                corrRow.push_back(termLabel);

                if (i < grade - 1) {
                    auto* op = new MathLabel(fontSet.medium, black, renderer, "+", 0, 0);
                    summaryLabels.push_back(op);
                    corrRow.push_back(op);
                }
            }
            pageBox->addRow(corrRow);
        }
    }
}