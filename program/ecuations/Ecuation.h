#ifndef ECUATION_H
#define ECUATION_H

#include <vector>
#include <random>
#include "../UI/TextBox.h"
#ifdef __EMSCRIPTEN__
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif
#include "../UI/MathLabel.h"
#include "../UI/ScrollBox.h"
#include "../UI/PageBox.h"
#include "../UI/FontSet.h"
#include <string>

struct AnswerSlot {
    int x, y, w, h;
    std::string hint;
};

class Ecuation {
protected:
    int centerX = 500;
    int centerY = 300;
    int hintX = 25;
    int hintY = 225;
public:

    std::vector<int> parameters;
    std::vector<TextBox> userAnswer;
    std::vector<double> correctAnswer;
    std::vector<double> givenAnswer;
    std::mt19937& engine;
    bool wasCorrect=false;
    bool hintChecked=false;

    Ecuation(std::mt19937& engine, int cx = 500, int cy = 300);
    virtual ~Ecuation();

    virtual void generateProblem() = 0;
    virtual void calculateAnswer() = 0;
    virtual void buildDisplay(SDL_Renderer* renderer, FontSet FontSet) = 0;
    virtual bool verifyAnswer(std::vector<double> answer);
    virtual std::vector<AnswerSlot> getAnswerSlots() = 0;
    virtual bool validateAnswer(){return true;}
    virtual void drawExtra(SDL_Renderer* renderer) {}
    virtual void addToScrollBox(ScrollBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) = 0;
    virtual void addToPageBox(PageBox* scrollBox, SDL_Renderer* renderer, FontSet FontSet) = 0;
};

#endif