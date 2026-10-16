#include "Galaga.h"
#include "GameContext.h"
#include "DrawContext.h"
#include "MathUtil.h"
#include "Keyboard.h"

Galaga::Galaga(int width, int height) : score(0), lives(3), wave(1) {
    windowSize = CMPUT350::Point2D(width, height);
}

void Galaga::Initialize(CMPUT350::GameContext *context) {
    // Load controls
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::A, "MoveLeft");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Left, "MoveLeft");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::D, "MoveRight");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Right, "MoveRight");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Space, "Shoot");
    context->mEngineView->InstallKeyUpNotification(CMPUT350::Key::A, "StopLeft");
    context->mEngineView->InstallKeyUpNotification(CMPUT350::Key::Left, "StopLeft");
    context->mEngineView->InstallKeyUpNotification(CMPUT350::Key::D, "StopRight");
    context->mEngineView->InstallKeyUpNotification(CMPUT350::Key::Right, "StopRight");

    // Game State controls
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Num1, "StartGame");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Numpad1, "StartGame");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Num5, "InsertCoin");
    context->mEngineView->InstallKeyDownNotification(CMPUT350::Key::Numpad5, "InsertCoin");

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
            context->currentFrame = 0;
            break;
        case GameState::Startup:
            // Display level number and load player.

            //todo

            if (context->currentFrame >= 300){
                context->currentFrame = 0;
                gameState = GameState::WaveStart;
            }
            break;
        case GameState::WaveStart:
            // Start a new wave of enemies

            //todo

            if (context->currentFrame >= 300){
                context->currentFrame = 0;
                gameState = GameState::WaveStart;
            }
            break;
        case GameState::Playing:
            // Main game loop logic (hopefully just main here)

            //todo

            // Set this to 0 only when the game ends, but to preserve gameloop we'll just reset it here 24/7
            if (context->currentFrame >= 300){
                context->currentFrame = 0;
                gameState = GameState::GameOver;
            }
            break;
        case GameState::GameOver:
            // Wait for small period and then return to waiting
            if (context->currentFrame >= 300){
                context->currentFrame = 0;
                gameState = GameState::Waiting;
            }
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