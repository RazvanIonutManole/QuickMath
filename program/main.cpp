
#include "SDL_Include.h"
#include "SDL_TTF_Include.h"
#include <emscripten.h>
#include <iostream>
#include "UI/ImageButton.h"
#include "UI/MathLabel.h"
#include "UI/TextBox.h"
#include "UI/ScrollBox.h"
#include "UI/FontSet.h"
#include "UI/Animation.h"
#include "UI/PageBox.h"
#include "UI/RadioGroup.h"
#include "ecuations/Addition.h"
#include "ecuations/Substraction.h"
#include "ecuations/Multiplication.h"
#include "ecuations/Division.h"
#include "ecuations/PolinomialG1.h"
#include "ecuations/FractionAdd.h"
#include "ecuations/FractionMultiply.h"
#include "ecuations/DecimalEcuation.h"
#include "ecuations/DerivativePol.h"
#include "ecuations/DerivativeTrig.h"
#include "ecuations/DerivativePolExp.h"
#include "ecuations/LimitInfPol.h"
#include "ecuations/LimitFindA.h"
#include "ecuations/LimitLinAdd.h"

#include <SDL2/SDL.h>

#include <emscripten/fetch.h>
#include <random>
#include <vector>
#include <map>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

ImageButton* startButton;
ImageButton* submitButton;
ImageButton* hintButton;

Animation* failAnimation;

MathLabel* scoreLabel;
MathLabel* timerLabel;
MathLabel* gameOverLabel;
MathLabel* hintLabel;
MathLabel* usernameLabel;
MathLabel* highScoreLabel;
MathLabel* acuracyLabel;
MathLabel* successRateLabel;

RadioGroup* difficultySettings;

std::vector<TextBox*> answerBox;

SDL_Texture* logo;
SDL_Rect logoRect;
SDL_Texture* background;
SDL_Rect fullScreen;
SDL_Texture* paper;
SDL_Rect paperRect;

SDL_Texture* mathNoise;
SDL_Rect mathNoiseRect;

FontSet fonts;

PageBox* statsBox;

Uint32 gameStartTime = 0;
Uint32 gameTimeLimit = 30000;
Uint32 lastRemaining = 0;

std::vector<Ecuation*> ecuations;

const std::string SERVER_URL = "http://localhost:5000";
bool OnLoginPage = false;
bool OnSignUpPage = false;
int loggedInUserId = -1;
std::string loggedInUsername = "";
std::map<std::string, int> loggedInHighScores;
std::string currentDifficulty = "basic";

Uint32 wrongAnswerTime = 0;
bool waitingAfterWrong = false;

// Login page
TextBox* loginUsernameBox = nullptr;
TextBox* loginPasswordBox = nullptr;
ImageButton* loginSubmitButton = nullptr;
ImageButton* loginBackButton = nullptr;
MathLabel* loginTitleLabel = nullptr;
MathLabel* loginErrorLabel = nullptr;
MathLabel* loginUsernameLabelText = nullptr;
MathLabel* loginPasswordLabelText = nullptr;

// Signup page
TextBox* signupUsernameBox = nullptr;
TextBox* signupPasswordBox = nullptr;
ImageButton* signupSubmitButton = nullptr;
ImageButton* signupBackButton = nullptr;
MathLabel* signupTitleLabel = nullptr;
MathLabel* signupErrorLabel = nullptr;
MathLabel* signupUsernameLabelText = nullptr;
MathLabel* signupPasswordLabelText = nullptr;

// Main menu buttons for navigation
ImageButton* goToLoginButton = nullptr;
ImageButton* goToSignupButton = nullptr;

bool GameOn = false;
bool GameOver = false;
int hintCount = 0;
int Score;
int questionsAnswered;
int acuracy;

static std::mt19937 gameEngine(std::random_device{}());

void onHighScoreSuccess(emscripten_fetch_t* fetch) {
    emscripten_fetch_close(fetch);
}
void onHighScoreFail(emscripten_fetch_t* fetch) {
    emscripten_fetch_close(fetch);
}
void sendHighScore() {
    if (loggedInUserId == -1) return;

    if (Score > loggedInHighScores[currentDifficulty])
        loggedInHighScores[currentDifficulty] = Score;

    // Use a persistent static body to keep memory stable during the background async fetch
    static std::string highScoreBody;
    highScoreBody = "{\"user_id\":" + std::to_string(loggedInUserId) +
                    ",\"score\":" + std::to_string(Score) +
                    ",\"difficulty\":\"" + currentDifficulty + "\"}";

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "POST");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onHighScoreSuccess;
    attr.onerror = onHighScoreFail;
    
    const char* headers[] = {"Content-Type", "application/json", nullptr};
    attr.requestHeaders = headers;
    
    // Safely point to the persistent static string contents
    attr.requestData = highScoreBody.c_str();
    attr.requestDataSize = highScoreBody.size();
    
    emscripten_fetch(&attr, "http://127.0.0.1:5000/highscore");
}

void setFocusedToDefault() {
    answerBox[0]->isFocused = true;
    for (int i = 1; i < (int)answerBox.size(); i++)
        answerBox[i]->isFocused = false;
}
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (newTexture == nullptr)
        std::cerr << "Failed to load texture: " << path << " | SDL_image Error: " << IMG_GetError() << std::endl;
    return newTexture;
}
void generateNextEcuation() {
    SDL_StopTextInput();

    int ecuationType;

    if (currentDifficulty=="basic"){
        std::uniform_int_distribution<int> dist(1, 4);
        ecuationType = dist(gameEngine);
        std::cout << "Ecuation: " << ecuationType << std::endl;
    }
    else if (currentDifficulty=="advanced"){
        std::uniform_int_distribution<int> dist(5, 8);
        ecuationType = dist(gameEngine);
        std::cout << "Ecuation: " << ecuationType << std::endl;
    }
    else if (currentDifficulty=="derivatives"){
        std::uniform_int_distribution<int> dist(9, 11);
        ecuationType = dist(gameEngine);
        std::cout << "Ecuation: " << ecuationType << std::endl;
    }
    else if (currentDifficulty=="limits"){
        std::uniform_int_distribution<int> dist(12, 14);
        ecuationType = dist(gameEngine);
        std::cout << "Ecuation: " << ecuationType << std::endl;
    }

    //ecuationType=11;

    switch (ecuationType) {
        case 1:  ecuations.push_back(new Addition(gameEngine));          break;
        case 2:  ecuations.push_back(new Substraction(gameEngine));      break;
        case 3:  ecuations.push_back(new Multiplication(gameEngine));    break;
        case 4:  ecuations.push_back(new Division(gameEngine));          break;
        case 5:  ecuations.push_back(new PolinomialG1(gameEngine));      break;
        case 6:  ecuations.push_back(new FractionAdd(gameEngine));       break;
        case 7:  ecuations.push_back(new FractionMultiply(gameEngine));  break;
        case 8:  ecuations.push_back(new DecimalEcuation(gameEngine));   break;
        case 9:  ecuations.push_back(new DerivativePol(gameEngine));     break;
        case 10: ecuations.push_back(new DerivativeTrig(gameEngine));    break;
        case 11: ecuations.push_back(new DerivativePolExp(gameEngine));  break;
        case 12: ecuations.push_back(new LimitInfPol(gameEngine));       break;
        case 13: ecuations.push_back(new LimitFindA(gameEngine));        break;
        case 14: ecuations.push_back(new LimitLinAdd(gameEngine));       break;
    }

    for (auto* box : answerBox) delete box;
    answerBox.clear();

    for (auto& slot : ecuations.back()->getAnswerSlots()) {
        auto* box = new TextBox(slot.x, slot.y, slot.w, slot.h, fonts.large, {0, 0, 0, 255}, slot.hint);
        box->initHint(renderer);
        answerBox.push_back(box);
    }
    setFocusedToDefault();
    SDL_StartTextInput();

    ecuations.back()->buildDisplay(renderer, fonts);
    scoreLabel->update(renderer, "Score: " + std::to_string(Score), 500, 200);
}

void onCorrectAnswer() {
    generateNextEcuation();
}

void onWrongAnswer() {
    waitingAfterWrong = true;
    wrongAnswerTime = SDL_GetTicks();
    failAnimation->reset();
}

void trySubmit() {
    bool allFilled = true;
    for (auto* box : answerBox)
        if (box->userInput.empty()) { allFilled = false; break; }

    if (!allFilled) setFocusedToDefault();

    if (allFilled) {
        try {
            questionsAnswered++;
            std::vector<double> playerAns;
            for (auto* box : answerBox)
                playerAns.push_back(std::stod(box->userInput));

            ecuations.back()->givenAnswer = playerAns;
            if (ecuations.back()->verifyAnswer(playerAns)) {
                Score++;
                onCorrectAnswer();
            } else {
                onWrongAnswer();
            }
        } catch (...) {
            for (auto* box : answerBox) box->clearInput();
        }
    }
}
void startGame() {
    int diff = difficultySettings->getSelected();
    if (diff == 0) currentDifficulty = "basic";
    else if (diff == 1) currentDifficulty = "advanced";
    else if (diff == 2) currentDifficulty = "derivatives";
    else if (diff == 3) currentDifficulty = "limits";
    if(diff==-1) return;
    std::cout << "Starting Game..." << std::endl;
    GameOn = true;
    GameOver = false;
    hintCount = 3;
    hintLabel->update(renderer, "Hints Remaining: " + std::to_string(hintCount),
                      hintButton->rect.x + 50, hintButton->rect.y - 30);
    Score = 0;
    lastRemaining = 0;
    questionsAnswered = 0;
    startButton->isEnabled = false;
    submitButton->isEnabled = true;
    hintButton->isEnabled = true;

    for (Ecuation* eq : ecuations) delete eq;
    ecuations.clear();

    gameStartTime = SDL_GetTicks();
    generateNextEcuation();
    SDL_StartTextInput();
}
void useHint() {
    answerBox[0]->isFocused = true;
    if (ecuations.back()->hintChecked == true) return;
    ecuations.back()->hintChecked = true;
    hintCount--;
    hintLabel->update(renderer, "Hints Remaining: " + std::to_string(hintCount),
                      hintButton->rect.x + 50, hintButton->rect.y - 30);
    if (hintCount == 0)
        hintButton->isEnabled = false;
}

void updateLocalHighScore(){
    difficultySettings->buttons[0].label="High Score: " + std::to_string(loggedInHighScores["basic"]);
    difficultySettings->buttons[1].label="High Score: " + std::to_string(loggedInHighScores["advanced"]);
    difficultySettings->buttons[2].label="High Score: " + std::to_string(loggedInHighScores["derivatives"]);
    difficultySettings->buttons[3].label="High Score: " + std::to_string(loggedInHighScores["limits"]);
    for(int i=0;i<=3;i++){
        difficultySettings->buttons[i].labelObj->update(renderer, 
            difficultySettings->buttons[i].label, 
            difficultySettings->buttons[i].x + difficultySettings->buttons[i].w / 2, 
            difficultySettings->buttons[i].y - 20);
        }
}

void gameOver() {
    GameOn = false;
    GameOver = true;
    startButton->isEnabled = true;
    submitButton->isEnabled = false;
    hintButton->isEnabled = false;
    gameOverLabel->update(renderer, "Final Score: " + std::to_string(Score), 500, 250);
    sendHighScore();
    loggedInHighScores[currentDifficulty] = std::max(loggedInHighScores[currentDifficulty], Score);

    updateLocalHighScore();

    highScoreLabel->update(renderer, "High Score: " + std::to_string(loggedInHighScores[currentDifficulty]), 500, 220);
    if (questionsAnswered == 0) acuracy = 0;
    else acuracy=100 * Score/questionsAnswered;
    if (acuracy<50) acuracyLabel->color = {255, 0, 0, 255};
    else if (acuracy<80) acuracyLabel->color = {150, 150, 0, 255};
    else acuracyLabel->color = {0, 255, 0, 255};
    acuracyLabel->update(renderer, "Acuracy: " + std::to_string(acuracy) + "%", 200, 200);
    successRateLabel->update(renderer, "Success Rate: " + std::to_string(Score) + "/" + std::to_string(questionsAnswered), 200, 300);

    statsBox->clear();
    for (int i = 0; i < (int)ecuations.size(); i++) {
        statsBox->nextQuestion(renderer, std::to_string(i + 1));
        ecuations[i]->addToPageBox(statsBox, renderer, fonts);
    }
    statsBox->finalizePage();
    SDL_StopTextInput();
}
std::string cleanUsername(std::string str) {
    // removes junk characters from usernames to correctly fetch from the database afterwards

    // Loop 1: Keep removing from the front UNTIL we remove the quote
    while (!str.empty()) {
        char frontChar = str.front();
        str.erase(0, 1); // Remove the front character
        
        if (frontChar == '"') {
            break; // We found and removed the quote! Stop looping.
        }
    }

    // Loop 2: Keep removing from the back UNTIL we remove the quote
    while (!str.empty()) {
        char backChar = str.back();
        str.pop_back(); // Remove the back character
        
        if (backChar == '"') {
            break; // We found and removed the quote! Stop looping.
        }
    }

    return str;
}
// --- Fetch callbacks ---
void onLoginSuccess(emscripten_fetch_t* fetch) {
    std::string response(fetch->data, fetch->numBytes);

    auto getField = [&](std::string key) -> std::string {
        std::string search = "\"" + key + "\":";
        size_t pos = response.find(search);
        if (pos == std::string::npos) return "0";
        
        pos += search.size();
        
        // Skip leading spaces, quotes, or formatting tabs
        while(pos < response.size() && (response[pos] == ' ' || response[pos] == '"' || response[pos] == '\t')) {
            pos++;
        }
        
        // Find where the value ends by looking out for delimiters
        size_t end = pos;
        while(end < response.size() && 
              response[end] != ',' && 
              response[end] != '}' && 
              response[end] != '"' && 
              response[end] != '\n' && 
              response[end] != '\r') {
            end++;
        }
        
        std::string result = response.substr(pos, end - pos);
        
        // Quick trim loop to remove any accidental trailing spaces
        while(!result.empty() && (result.back() == ' ' || result.back() == '\r' || result.back() == '\n')) {
            result.pop_back();
        }
        
        return result.empty() ? "0" : result;
    };

    try {
        loggedInUserId = std::stoi(getField("id"));
        loggedInUsername = getField("username");
        usernameLabel->update(renderer, loggedInUsername, 900, 580);
        
        loggedInHighScores["basic"]       = std::stoi(getField("basic"));
        loggedInHighScores["advanced"]    = std::stoi(getField("advanced"));
        loggedInHighScores["derivatives"] = std::stoi(getField("derivatives"));
        loggedInHighScores["limits"]      = std::stoi(getField("limits"));
        
        updateLocalHighScore();

        OnLoginPage = false;
    } catch (...) {
        loginErrorLabel->update(renderer, "Server response error. Try again.", 500, 390);
    }

    emscripten_fetch_close(fetch);
}

void onLoginFail(emscripten_fetch_t* fetch) {
    loginErrorLabel->update(renderer, "Invalid username or password", 500, 390);
    emscripten_fetch_close(fetch);
}

void onSignupSuccess(emscripten_fetch_t* fetch) {
    OnSignUpPage = false;
    emscripten_fetch_close(fetch);
}

void onSignupFail(emscripten_fetch_t* fetch) {
    signupErrorLabel->update(renderer, "Username already exists or server error", 500, 390);
    emscripten_fetch_close(fetch);
}

void sendSignup() {
    std::string user = signupUsernameBox->userInput;
    std::string pass = signupPasswordBox->userInput;
    if (user.empty() || pass.empty()) {
        signupErrorLabel->update(renderer, "Please fill in all fields", 500, 450);
        return;
    }

    // 1. We use a static variable so its memory context remains stable 
    // throughout the duration of the background browser fetch execution.
    static std::string signupBody; 
    signupBody = "{\"username\":\"" + user + "\",\"password\":\"" + pass + "\"}";

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "POST");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onSignupSuccess;
    attr.onerror = onSignupFail;
    
    const char* headers[] = {"Content-Type", "application/json", nullptr};
    attr.requestHeaders = headers;
    
    // 2. Safely bind to the persistent string reference
    attr.requestData = signupBody.c_str();
    attr.requestDataSize = signupBody.size();
    
    emscripten_fetch(&attr, "http://127.0.0.1:5000/register");
}

void sendLogin() {
    std::string user = loginUsernameBox->userInput;
    std::string pass = loginPasswordBox->userInput;
    if (user.empty() || pass.empty()) {
        loginErrorLabel->update(renderer, "Please fill in all fields", 500, 450);
        return;
    }

    static std::string loginBody;
    loginBody = "{\"username\":\"" + user + "\",\"password\":\"" + pass + "\"}";

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "POST");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onLoginSuccess;
    attr.onerror = onLoginFail;
    
    const char* headers[] = {"Content-Type", "application/json", nullptr};
    attr.requestHeaders = headers;
    
    attr.requestData = loginBody.c_str();
    attr.requestDataSize = loginBody.size();
    
    emscripten_fetch(&attr, "http://127.0.0.1:5000/login");
}

void main_loop() {
    SDL_Event event;

    // =====================
    // INPUT PHASE
    // =====================
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            emscripten_cancel_main_loop();
            return;
        }

        if (!GameOn && !GameOver && !OnLoginPage && !OnSignUpPage) {
            if (startButton->isClicked(event)) startGame();
            if (goToLoginButton->isClicked(event)) {
                OnLoginPage = true;
                loginUsernameBox->clearInput();
                loginPasswordBox->clearInput();
                SDL_StartTextInput();
            }
            if (goToSignupButton->isClicked(event)) {
                OnSignUpPage = true;
                signupUsernameBox->clearInput();
                signupPasswordBox->clearInput();
                SDL_StartTextInput();
            }
            difficultySettings->handleEvent(event);
        }

        if (OnLoginPage) {
            if (loginBackButton->isClicked(event)) OnLoginPage = false;
            if (loginSubmitButton->isClicked(event)) sendLogin();
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) sendLogin();
            loginUsernameBox->handleEvent(event);
            loginPasswordBox->handleEvent(event);
        }

        if (OnSignUpPage) {
            if (signupBackButton->isClicked(event)) OnSignUpPage = false;
            if (signupSubmitButton->isClicked(event)) sendSignup();
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) sendSignup();
            signupUsernameBox->handleEvent(event);
            signupPasswordBox->handleEvent(event);
        }

        if (GameOn && !waitingAfterWrong) {
            if (submitButton->isClicked(event)) trySubmit();
            if (hintButton->isClicked(event)) useHint();
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) trySubmit();
                if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_TAB) {
                    for (int i = 0; i < (int)answerBox.size(); i++) {
                        if (answerBox[i]->isFocused) {
                            answerBox[i]->isFocused = false;
                            answerBox[(i + 1) % answerBox.size()]->isFocused = true;
                            break;
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_LEFT) {
                    for (int i = 0; i < (int)answerBox.size(); i++) {
                        if (answerBox[i]->isFocused) {
                            answerBox[i]->isFocused = false;
                            answerBox[(i - 1 + answerBox.size()) % answerBox.size()]->isFocused = true;
                            break;
                        }
                    }
                }
            }
            for (auto* box : answerBox) box->handleEvent(event);
        }

        if (GameOver) {
            if (startButton->isClicked(event)) GameOver = false;
            statsBox->handleEvent(event);
        }
    }

    // =====================
    // UPDATE PHASE
    // =====================
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    if (!GameOn && !GameOver && !OnLoginPage && !OnSignUpPage) {
        if (startButton)     startButton->updateHover(mx, my);
        if (goToLoginButton)  goToLoginButton->updateHover(mx, my);
        if (goToSignupButton) goToSignupButton->updateHover(mx, my);
    }
    if (OnLoginPage) {
        loginBackButton->updateHover(mx, my);
        loginSubmitButton->updateHover(mx, my);
    }
    if (OnSignUpPage) {
        signupBackButton->updateHover(mx, my);
        signupSubmitButton->updateHover(mx, my);
    }
    if (GameOn) {
        if (submitButton && submitButton->isEnabled) submitButton->updateHover(mx, my);
        if (hintButton && hintButton->isEnabled)     hintButton->updateHover(mx, my);

        if(!failAnimation->finished){
            failAnimation->update();
        }
        if (waitingAfterWrong) {
            if (SDL_GetTicks() - wrongAnswerTime >= 1000) {
                waitingAfterWrong = false;
                generateNextEcuation();
            }
        }

        Uint32 elapsed = SDL_GetTicks() - gameStartTime;
        if (elapsed >= gameTimeLimit) {
            gameOver();
        } else {
            Uint32 remaining = (gameTimeLimit - elapsed) / 1000 + 1;
            if (remaining != lastRemaining) {
                timerLabel->update(renderer, "Time: " + std::to_string(remaining), 500, 100);
                lastRemaining = remaining;
            }
        }
    }
    if (GameOver) {
        if (startButton && startButton->isEnabled) startButton->updateHover(mx, my);
    }

    // =====================
    // DRAW PHASE
    // =====================
    
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, &fullScreen);
    

    // login page - clears and draws its own screen
    if (OnLoginPage) {
        loginTitleLabel->draw(renderer);
        loginUsernameLabelText->draw(renderer);
        loginPasswordLabelText->draw(renderer);
        loginUsernameBox->draw(renderer);
        loginPasswordBox->draw(renderer);
        loginErrorLabel->draw(renderer);
        loginSubmitButton->draw(renderer);
        loginBackButton->draw(renderer);
        SDL_RenderPresent(renderer);
        return;
    }

    // signup page - clears and draws its own screen
    if (OnSignUpPage) {
        signupTitleLabel->draw(renderer);
        signupUsernameLabelText->draw(renderer);
        signupPasswordLabelText->draw(renderer);
        signupUsernameBox->draw(renderer);
        signupPasswordBox->draw(renderer);
        signupErrorLabel->draw(renderer);
        signupSubmitButton->draw(renderer);
        signupBackButton->draw(renderer);
        SDL_RenderPresent(renderer);
        return;
    }

    // logo
    if (GameOn)
        logoRect = SDL_Rect{10, 10, 200, 40};
    else if(GameOver)
        logoRect = SDL_Rect{10, 10, 500, 100};
    else logoRect = SDL_Rect{10, 10, 1000, 200};
    SDL_RenderCopy(renderer, logo, NULL, &logoRect);

    

    // username label (always visible when logged in)
    if (usernameLabel) usernameLabel->draw(renderer);

    // main menu
    if (!GameOn && !GameOver && !OnSignUpPage && !OnLoginPage) {
        if (startButton && startButton->isEnabled) startButton->draw(renderer);
        if (goToLoginButton)  goToLoginButton->draw(renderer);
        if (goToSignupButton) goToSignupButton->draw(renderer);
        if (difficultySettings) difficultySettings->draw(renderer);
    }

    // game screen
    if (GameOn) {
        if (scoreLabel) scoreLabel->draw(renderer);
        for (auto* box : answerBox) box->draw(renderer);
        if (submitButton) submitButton->draw(renderer);
        if (hintButton)   hintButton->draw(renderer);
        timerLabel->draw(renderer);
        SDL_RenderCopy(renderer, mathNoise, NULL, &mathNoiseRect);
        if(ecuations.back()->hintChecked){
            SDL_SetRenderDrawColor(renderer, 0, 0, 80, 255);
            SDL_Rect tempRect = {10, 180, 350, 350};
            SDL_RenderFillRect(renderer, &tempRect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            tempRect = {20, 190, 330, 330};
            SDL_RenderFillRect(renderer, &tempRect);
        }
        if (!ecuations.empty()) ecuations.back()->drawExtra(renderer);
        hintLabel->draw(renderer);
        
        if(!failAnimation->finished)
            failAnimation->draw(renderer, 485, 135, 30, 30);
    }

    // game over screen
    if (GameOver) {
        gameOverLabel->draw(renderer);
        highScoreLabel->draw(renderer);
        acuracyLabel->draw(renderer);
        successRateLabel->draw(renderer);
        SDL_RenderCopy(renderer, paper, NULL, &paperRect);
        SDL_RenderCopy(renderer, mathNoise, NULL, &mathNoiseRect);
        statsBox->draw(renderer);
        if (startButton && startButton->isEnabled) startButton->draw(renderer);
    }

    SDL_RenderPresent(renderer);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s/n", TTF_GetError());
        return 1;
    }
    SDL_CreateWindowAndRenderer(1000, 600, 0, &window, &renderer);

    background = loadTexture("images/background.png", renderer);
    fullScreen= SDL_Rect{0, 0, 1000, 600};

    SDL_Texture* startNormal   = loadTexture("images/Start.png", renderer);
    SDL_Texture* startPressed  = loadTexture("images/StartPressed.png", renderer);
    SDL_Texture* hint          = loadTexture("images/Hint.png", renderer);
    SDL_Texture* hintPressed   = loadTexture("images/HintPressed.png", renderer);
    SDL_Texture* hintDisabled  = loadTexture("images/HintDisabled.png", renderer);
    SDL_Texture* loginImage  = loadTexture("images/LoginButton.png", renderer);
    SDL_Texture* loginPressedImage  = loadTexture("images/LoginButtonPressed.png", renderer);
    SDL_Texture* signUpImage  = loadTexture("images/SignUpButton.png", renderer);
    SDL_Texture* signUpPressedImage  = loadTexture("images/SignUpButtonPressed.png", renderer);
    SDL_Texture* backImage  = loadTexture("images/BackButton.png", renderer);
    SDL_Texture* backPressedImage  = loadTexture("images/BackButtonPressed.png", renderer);

    SDL_Texture* fundamentalsSelected = loadTexture("images/fundamentalsSelected.png", renderer);
    SDL_Texture* fundamentalsUnselected = loadTexture("images/fundamentalsUnselected.png", renderer);
    SDL_Texture* advancedSelected = loadTexture("images/advancedSelected.png", renderer);
    SDL_Texture* advancedUnselected = loadTexture("images/advancedUnselected.png", renderer);
    SDL_Texture* derivativesSelected = loadTexture("images/derivativesSelected.png", renderer);
    SDL_Texture* derivativesUnselected = loadTexture("images/derivativesUnselected.png", renderer);
    SDL_Texture* limitsSelected = loadTexture("images/limitsSelected.png", renderer);
    SDL_Texture* limitsUnselected = loadTexture("images/limitsUnselected.png", renderer);

    mathNoise = loadTexture("images/mathNoise.png", renderer);
    mathNoiseRect = {20, 330, 400, 250};

    failAnimation = new Animation(
        {
            loadTexture("images/failAnimation/failAnimation1.png", renderer),
            loadTexture("images/failAnimation/failAnimation2.png", renderer),
            loadTexture("images/failAnimation/failAnimation3.png", renderer),
            loadTexture("images/failAnimation/failAnimation4.png", renderer),
            loadTexture("images/failAnimation/failAnimation5.png", renderer),
            loadTexture("images/failAnimation/failAnimation6.png", renderer),
            loadTexture("images/failAnimation/failAnimation7.png", renderer),
            loadTexture("images/failAnimation/failAnimation8.png", renderer),
            loadTexture("images/failAnimation/failAnimation9.png", renderer),
            loadTexture("images/failAnimation/failAnimation10.png", renderer),
            loadTexture("images/failAnimation/failAnimation11.png", renderer),
            loadTexture("images/failAnimation/failAnimation12.png", renderer),
            loadTexture("images/failAnimation/failAnimation13.png", renderer),
            loadTexture("images/failAnimation/failAnimation14.png", renderer),
            loadTexture("images/failAnimation/failAnimation15.png", renderer),
            loadTexture("images/failAnimation/failAnimation16.png", renderer),
            loadTexture("images/failAnimation/failAnimation17.png", renderer),
            loadTexture("images/failAnimation/failAnimation18.png", renderer),
            loadTexture("images/failAnimation/failAnimation19.png", renderer),
            loadTexture("images/failAnimation/failAnimation20.png", renderer),
        },
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}, 
        0, 20
    );
    failAnimation->finished=true;
    failAnimation->loop=false;

    logo = loadTexture("images/Logo.png", renderer);
    paper = loadTexture("images/paper.png", renderer);

    startButton  = new ImageButton("start",  450, 275, 100, 60, startNormal, startPressed, startNormal);
    submitButton = new ImageButton("submit", 450, 500, 100, 60, startNormal, startPressed, startNormal);
    hintButton   = new ImageButton("hint",    70, 100, 100, 60, hint, hintPressed, hintDisabled);

    goToLoginButton  = new ImageButton("goToLogin",  50, 500, 100, 60, loginImage, loginPressedImage, loginImage);
    goToSignupButton = new ImageButton("goToSignup", 150, 500, 100, 60, signUpImage, signUpPressedImage, signUpImage);

    fonts.large      = TTF_OpenFont("fonts/INKFREE.TTF", 24);
    fonts.medium     = TTF_OpenFont("fonts/INKFREE.TTF", 18);
    fonts.small      = TTF_OpenFont("fonts/INKFREE.TTF", 13);
    fonts.largeSuper = TTF_OpenFont("fonts/INKFREE.TTF", 14);
    fonts.mediumSuper= TTF_OpenFont("fonts/INKFREE.TTF", 11);
    fonts.smallSuper = TTF_OpenFont("fonts/INKFREE.TTF",  8);

    scoreLabel       = new MathLabel(fonts.large, {0, 0, 0, 255});
    timerLabel       = new MathLabel(fonts.large, {0, 0, 0, 255});
    gameOverLabel    = new MathLabel(fonts.large, {0, 0, 0, 255});
    hintLabel        = new MathLabel(fonts.large, {0, 0, 0, 255});
    acuracyLabel     = new MathLabel(fonts.large, {0, 0, 0, 255});
    successRateLabel = new MathLabel(fonts.large, {0, 0, 0, 255});

    statsBox = new PageBox(650, 100, 300, 450, 70, fonts);
    paperRect = SDL_Rect{625, 80, 350, 490};

    usernameLabel = new MathLabel(fonts.large, {0, 0, 0, 255});
    highScoreLabel = new MathLabel(fonts.large, {0, 0, 0, 255});

    difficultySettings = new RadioGroup();
    difficultySettings->addButton(650, 375, 100, 60, fundamentalsUnselected, fundamentalsSelected, "High Score: " + std::to_string(loggedInHighScores["basic"]), renderer, fonts.large, {0, 0, 0, 255});
    difficultySettings->addButton(800, 375, 100, 60, advancedUnselected, advancedSelected, "High Score: " + std::to_string(loggedInHighScores["advanced"]), renderer, fonts.large, {0, 0, 0, 255});
    difficultySettings->addButton(650, 475, 100, 60, derivativesUnselected, derivativesSelected, "High Score: " + std::to_string(loggedInHighScores["derivatives"]), renderer, fonts.large, {0, 0, 0, 255});
    difficultySettings->addButton(800, 475, 100, 60, limitsUnselected, limitsSelected, "High Score: " + std::to_string(loggedInHighScores["limits"]), renderer, fonts.large, {0, 0, 0, 255});

    // login page
    loginTitleLabel        = new MathLabel(fonts.large, {0, 0, 0, 255});
    loginTitleLabel->update(renderer, "Login", 500, 100);
    loginUsernameLabelText = new MathLabel(fonts.large, {0, 0, 0, 255});
    loginUsernameLabelText->update(renderer, "Username:", 300, 250);
    loginPasswordLabelText = new MathLabel(fonts.large, {0, 0, 0, 255});
    loginPasswordLabelText->update(renderer, "Password:", 300, 350);
    loginUsernameBox = new TextBox(450, 225, 200, 40, fonts.large, {0, 0, 0, 255}, "", "", 20);
    loginPasswordBox = new TextBox(450, 325, 200, 40, fonts.large, {0, 0, 0, 255}, "", "", 30);
    loginPasswordBox->masked = true;
    loginErrorLabel  = new MathLabel(fonts.large, {220, 0, 0, 255});
    loginSubmitButton = new ImageButton("loginSubmit", 400, 420, 100, 60, loginImage, loginPressedImage, loginImage);
    loginBackButton   = new ImageButton("loginBack",    50,  50, 100, 60, backImage, backPressedImage, backImage);

    // signup page
    signupTitleLabel        = new MathLabel(fonts.large, {0, 0, 0, 255});
    signupTitleLabel->update(renderer, "Sign Up", 500, 100);
    signupUsernameLabelText = new MathLabel(fonts.large, {0, 0, 0, 255});
    signupUsernameLabelText->update(renderer, "Username:", 300, 250);
    signupPasswordLabelText = new MathLabel(fonts.large, {0, 0, 0, 255});
    signupPasswordLabelText->update(renderer, "Password:", 300, 350);
    signupUsernameBox = new TextBox(450, 225, 200, 40, fonts.large, {0, 0, 0, 255}, "", "", 20);
    signupPasswordBox = new TextBox(450, 325, 200, 40, fonts.large, {0, 0, 0, 255}, "", "", 30);
    signupPasswordBox->masked = true;
    signupErrorLabel  = new MathLabel(fonts.large, {255, 0, 0, 255});
    signupSubmitButton = new ImageButton("signupSubmit", 400, 420, 100, 60, signUpImage, signUpPressedImage, signUpImage);
    signupBackButton   = new ImageButton("signupBack",    50,  50, 100, 60, backImage, backPressedImage, backImage);

    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}