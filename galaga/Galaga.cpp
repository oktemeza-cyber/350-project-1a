#include "Galaga.h"
#include "GameContext.h"
#include "DrawContext.h"
#include "MathUtil.h"
#include "Keyboard.h"
#include "Player.h"
#include "Stars.h"
#include "Enemy.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"

#include <memory>
#include <string>
#include <iostream>
#include <vector>

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
    gameState = GameState::LoadStars;
}

void Galaga::Update(CMPUT350::GameContext *context) {
    switch (gameState) { // Fancy if statement, bunch of else ifs.
        case GameState::LoadStars: {
            context->mEngineView->AddGameObject(std::make_shared<Stars>(250, CMPUT350::Rect(0, 0, 768, 1024)));
            gameState = GameState::Waiting;
            break;
        }
        case GameState::Waiting: {
            // Waiting, do nothing
            context->GUIContext->DrawCenteredText("Galaga!!", 35, (windowSize * 0.5f) - CMPUT350::Point2D(0, 100), CMPUT350::Colors::white);

            if (!coinInserted){
                context->GUIContext->DrawCenteredText("Insert Coin", 30, windowSize * 0.5f, CMPUT350::Colors::yellow);
            } else {
                context->GUIContext->DrawCenteredText("Press Play", 30, windowSize * 0.5f, CMPUT350::Colors::yellow);
            }

            context->currentFrame = 0;
            break;
        }
        case GameState::Startup: {
            // Display level number and load player.
            bool playerExists = false;

            for (const auto& gameObject : *context->mEngineView) {
                if (std::dynamic_pointer_cast<Player>(gameObject)) {
                    playerExists = true;
                    break;
                }
            }

            if (!playerExists) {
                auto player = std::make_shared<Player>(
                    CMPUT350::Point2D(768 / 2, 900)
                );
                player->setSelf(player);
                context->mEngineView->AddGameObject(player);
            }

            context->GUIContext->DrawCenteredText("Level " + std::to_string(wave), 30, (windowSize * 0.5f) - CMPUT350::Point2D(0, 100), CMPUT350::Colors::white);
            context->GUIContext->DrawCenteredText(std::to_string(score), 30, windowSize * 0.5f, CMPUT350::Colors::yellow);

            if (context->currentFrame >= 150){
                gameState = GameState::WaveStart;
            }
            break;
        }
        case GameState::WaveStart: {
            // Start a new wave of enemies
            
            const int count = 8; //enemy count
            const float spacing = 768.0f / (count + 1);
            for (int x = 0; x < count; x++)
            {
                CMPUT350::Point2D slot(spacing * (x + 1), 100);

                std::vector<CMPUT350::Point2D> path;
                path.push_back(CMPUT350::Point2D(slot.x, -50.0f));
                path.push_back(CMPUT350::Point2D(slot.x, 0.0f));
                path.push_back(CMPUT350::Point2D(slot.x - 60.0f, 300.0f));
                path.push_back(slot);

                auto enemy = std::make_shared<Enemy1>(slot, path, randomizer.GetRand());
                context->mEngineView->AddGameObject(enemy);
            }

            for (int x = 0; x < count; x++)
            {
                CMPUT350::Point2D slot(spacing * (x + 1), 200);

                std::vector<CMPUT350::Point2D> path;
                path.push_back(CMPUT350::Point2D(slot.x, -20.0f));
                path.push_back(CMPUT350::Point2D(slot.x, 70.0f));
                path.push_back(CMPUT350::Point2D(slot.x - 60.0f, 300.0f));
                path.push_back(slot);

                auto enemy2 = std::make_shared<Enemy2>(slot, path, randomizer.GetRand());
                context->mEngineView->AddGameObject(enemy2);
            }

            for (int x = 0; x < count; x++)
            {
                CMPUT350::Point2D slot(spacing * (x + 1), 300);

                std::vector<CMPUT350::Point2D> path;
                path.push_back(CMPUT350::Point2D(slot.x -100.0f, -50.0f));
                path.push_back(CMPUT350::Point2D(slot.x + 150.0f, 70.0f));
                path.push_back(CMPUT350::Point2D(slot.x, 300.0f));
                path.push_back(slot);

                auto enemy3 = std::make_shared<Enemy3>(slot, path, randomizer.GetRand());
                context->mEngineView->AddGameObject(enemy3);
            }
        

            context->currentFrame = 0;
            gameState = GameState::Playing;
            break;
        }
        case GameState::Playing: {
            // Main game loop logic
            context->GUIContext->DrawCenteredText(std::to_string(score), 20, (windowSize * 0.5f) - CMPUT350::Point2D(0, 500), CMPUT350::Colors::yellow);

            int remainingEnemies = 0;
            bool playerExists = false;
            bool enemiesExist = false;  
            std::weak_ptr<Player> playerPoint;

            for (const auto& gameObject : *context->mEngineView) {
                auto player = std::dynamic_pointer_cast<Player>(gameObject);
                if (player) {
                    playerExists = true;
                    playerPoint = player;
                }
            }

            for (const auto& gameObject : *context->mEngineView) {
                auto enemy = std::dynamic_pointer_cast<Enemy>(gameObject);
                if (enemy) {
                    enemiesExist = true;
                    remainingEnemies++;
                    if (playerExists){
                        enemy->Attack(context, playerPoint.lock()->GetLocation(), randomizer.GetRand());
                    } else {
                        enemy->PauseAttacking();
                    }
                }
            }

            score = (wave - 1) * 24 * 50 + (24 - remainingEnemies) * 50;

            if (!playerExists) {
                --lives;

                if (lives <= 0) {
                    context->currentFrame = 0;
                    gameState = GameState::GameOver;
                    break;
                }

                respawnTimer = context->currentFrame + 180;
                gameState = GameState::Respawn;
            } else if (!enemiesExist) {
                if (wave >= 5) {
                    context->currentFrame = 0;
                    gameState = GameState::Winning;
                } else {
                    ++wave;
                    context->currentFrame = 0;
                    gameState = GameState::Startup;
                }
            }

            break;
        }
        case GameState::Respawn: {
            context->GUIContext->DrawCenteredText("Lives left: " + std::to_string(lives), 30, (windowSize * 0.5f) - CMPUT350::Point2D(0, 100), CMPUT350::Colors::white);
            context->GUIContext->DrawCenteredText(std::to_string(score), 30, windowSize * 0.5f, CMPUT350::Colors::yellow);

            if (context->currentFrame >= respawnTimer){
                auto player = std::make_shared<Player>(
                    CMPUT350::Point2D(768 / 2, 900)
                );
                player->setSelf(player);
                context->mEngineView->AddGameObject(player);

                for (const auto& gameObject : *context->mEngineView) {
                    auto enemy = std::dynamic_pointer_cast<Enemy>(gameObject);
                    if (enemy) {
                        enemy->UnpauseAttacking();
                    }
                }

                gameState = GameState::Playing;
            }
            break;
        }
        case GameState::GameOver: {
            // Wait for small period and then return to waiting
            for (const auto& gameObject : *context->mEngineView) {
                if (std::dynamic_pointer_cast<Enemy>(gameObject)) {
                    gameObject->Kill();
                }
            }

            context->GUIContext->DrawCenteredText("Game Over", 30, (windowSize * 0.5f) - CMPUT350::Point2D(0, 100), CMPUT350::Colors::red);
            context->GUIContext->DrawCenteredText(std::to_string(score), 30, windowSize * 0.5f, CMPUT350::Colors::yellow);

            if (context->currentFrame >= 150){
                context->currentFrame = 0;
                gameState = GameState::Waiting;
            }
            break;
        }
        case GameState::Winning: {
            // Wait for small period and then return to waiting
            context->GUIContext->DrawCenteredText("You won!", 30, (windowSize * 0.5f) - CMPUT350::Point2D(0, 100), CMPUT350::Colors::white);
            context->GUIContext->DrawCenteredText(std::to_string(score), 30, windowSize * 0.5f, CMPUT350::Colors::yellow);

            if (context->currentFrame >= 150){
                context->currentFrame = 0;
                gameState = GameState::Waiting;
            }
            break;
        }
    }
}

void Galaga::ReceiveNotification(const std::string &key) {
    if (key == "StartGame" && coinInserted && gameState == GameState::Waiting) {
        // Reset game state variables
        score = 0;
        wave = 1;
        lives = 3;

        coinInserted = false;
        gameState = GameState::Startup;
    } else if (key == "InsertCoin" && gameState == GameState::Waiting) {
        coinInserted = true;
    }
}