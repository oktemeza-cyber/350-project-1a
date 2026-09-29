#include "GameEngine.h"
#include <memory>
#include "CollisionObject.h"
#include "GraphicsObject.h"
#include "DrawContext.h"
#include "GameContext.h"

/// @brief
namespace CMPUT350 {
#include "FontData.h"

void GameEngine::RemoveDead() {
    // Taken from asteroids
    std::vector<std::shared_ptr<GameObject>> aux;
    aux.reserve(mGameObjects.size());
    for (const auto& obj : mGameObjects) {
        if (obj->IsAlive()) {
            aux.push_back(obj);
        }
    }
    aux.swap(mGameObjects);
}

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name) {
    // Sample font loading code
    mFont = std::make_shared<sf::Font>();
    if (!mFont->openFromMemory(&_font, _font_len))
    {
        fprintf(stderr, "WARNING: Font did not load.\n");
    }

    mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(sf::Vector2u(width, height)), name); //I think this is how a window is setup
    mWindow->setFramerateLimit(30);
    mWindow->setKeyRepeatEnabled(false);
}

GameEngine::~GameEngine() {
    // Cleanup resources
    mWindow->close();
}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    mPending.push_back(gameObject);
}

/**
 * @method Run
 * @arguments None
 * @description Gives control to the game engine. Will not return until the game window is closed or
 * all objects have been destroyed.
 */
void GameEngine::Run() {
    DrawContext mDContext(mWindow, mFont);

    GameContext mGContext;
    mGContext.mEngineView = this;
    mGContext.ScreenContext = &mDContext;

    while (mWindow->isOpen())  // window is open
    {
        // 0. Remove any objects that are now dead
        RemoveDead();

        // 1. Activate and initialize any objects added during the last frame
        for (auto& newObj : mPending) {
            newObj->Initialize(&mGContext);
            mGameObjects.push_back(newObj);
        }
        mPending.clear();

        // 2. Process events
        GameEngine::ProcessEvents(&mGContext);

        // 3. Update game objects
        for (auto& obj : mGameObjects) {
            obj->Update(&mGContext);
        }

        // 4. Process collision events
        for (size_t i = 0; i < mGameObjects.size(); i++){ //Straight from description
            std::shared_ptr<CollisionObject> objA = std::dynamic_pointer_cast<CollisionObject>(mGameObjects[i]);
            if (objA == nullptr) continue; // Not a collision object, skip
            for (size_t j = 0; j < mGameObjects.size(); j++){
                std::shared_ptr<CollisionObject> objB = std::dynamic_pointer_cast<CollisionObject>(mGameObjects[j]);
                if (objB == nullptr) continue;
                if (i == j) continue;
                objA->CollisionEnter(objB);
            }
        }

        // 5. Late updates
        for (auto& obj : mGameObjects) {
            obj->LateUpdate(&mGContext);
        }

        // Clear window
        mWindow->clear(sf::Color::Black);

        // 6. Render background
        for (size_t i = 0; i < mGameObjects.size(); i++){
            std::shared_ptr<GraphicsObject> objA = std::dynamic_pointer_cast<GraphicsObject>(mGameObjects[i]);
            if (objA == nullptr) continue; // Not a graphics object, skip
            objA->RenderBackground(&mGContext);
        }

        // 7. Render foreground
        for (size_t i = 0; i < mGameObjects.size(); i++){
            std::shared_ptr<GraphicsObject> objA = std::dynamic_pointer_cast<GraphicsObject>(mGameObjects[i]);
            if (objA == nullptr) continue; // Not a graphics object, skip
            objA->RenderForeground(&mGContext);
        }

        // Actually render to window
        mWindow->display();

        if (mGameObjects.empty()) break;
    }
}

// Sample code for processing events

bool GameEngine::ProcessEvents(GameContext *context)
{
	while (const std::optional event = mWindow->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            delete this;
            return true;
        }
        else if (event->is<sf::Event::Resized>())
        {
            mWindow->setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resized->size)))) //...I think?
            return true;
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
        {
            // use keyPressed->unicode to get character
            for (auto& obj : mGameObjects) {
                obj->HandleKeyEvent(context, keyPressed->unicode);
            }
            return true;
        }
        return false;
    }
}


}  // namespace CMPUT350
