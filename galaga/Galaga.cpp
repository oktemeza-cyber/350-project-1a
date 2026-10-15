#include "Galaga.h"
#include "GameContext.h"
#include "DrawContext.h"
#include "MathUtil.h"

Galaga::Galaga(int width, int height) : score(0), lives(3), wave(1) {
    windowSize = CMPUT350::Point2D(width, height);
}

void Galaga::Initialize(CMPUT350::GameContext *context) {
    // Load controls
    context->mEngineView->InstallKeyDownNotification('A', "MoveLeft");
    context->mEngineView->InstallKeyDownNotification('D', "MoveRight");
    context->mEngineView->InstallKeyDownNotification(' ', "Shoot");
    context->mEngineView->InstallKeyUpNotification('A', "StopLeft");
    context->mEngineView->InstallKeyUpNotification('D', "StopRight");

    // Game State controls
    context->mEngineView->InstallKeyDownNotification('1', "StartGame");
    context->mEngineView->InstallKeyDownNotification('5', "InsertCoin");

    // Game state listeners
    context->mNotificationManager->Register(context->CurrObject, "StartGame");
    context->mNotificationManager->Register(context->CurrObject, "InsertCoin");

    coinInserted = false;
    gameState = GameState::Waiting;
}

void Galaga::Update(CMPUT350::GameContext *context) {
    switch (gameState) { // Fancy if statement, bunch of else ifs.
        case GameState::Waiting:
            // Waiting, do nothing
            break;
        case GameState::Startup:
            // Display level number and load player.

            //todo
            break;
        case GameState::WaveStart:
            // Start a new wave of enemies

            //todo
            break;
        case GameState::Playing:
            // Main game loop logic (hopefully just main here)

            //todo
            break;
        case GameState::GameOver:
            // Wait for small period and then return to waiting
            break;
    }
}

void Galaga::ReceiveNotification(const std::string &key) {
    if (key == "StartGame" && coinInserted && gameState == GameState::Waiting) {
        // Reset game state variables
        score = 0;
        lives = 3;

        coinInserted = false;
        gameState = GameState::Startup;
    } else if (key == "InsertCoin" && gameState == GameState::Waiting) {
        coinInserted = true;
    }
}