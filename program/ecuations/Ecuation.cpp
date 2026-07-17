#include "Ecuation.h"

Ecuation::Ecuation(std::mt19937& engine, int cx, int cy) 
    : engine(engine), centerX(cx), centerY(cy) {}

Ecuation::~Ecuation() {
}

bool Ecuation::verifyAnswer(std::vector<double> answer) {
    if (answer.size() != correctAnswer.size()) { wasCorrect = false; return false; }
    for (int i = 0; i < correctAnswer.size(); i++) {
        if (std::abs(answer[i] - correctAnswer[i]) > 0.001) { wasCorrect = false; return false; }
    }
    wasCorrect = true;
    return true;
}