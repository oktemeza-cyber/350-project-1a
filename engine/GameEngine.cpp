#include "GameEngine.h"

/// @brief
namespace CMPUT350 {
#include "FontData.h"

void RemoveDead() {
    // Taken from asteroids
    std::vector<shared_ptr<GameObject>> aux;
    aux.reserve(mGameObjects.size());
    for (const auto& obj : mGameObjects) {
        if (obj.isAlive) {
            aux.push_back(obj);
        }
    }
    aux.swap(mGameObjects);
}

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name) {
    // Sample font loading code
    if (!mFont->openFromMemory(&_font, _font_len)) // I uncommented this, but I'm not sure if it needs more work
    {
        fprintf(stderr, "WARNING: Font did not load.\n");
    }

    sf::RenderWindow mWindow(sf::VideoMode(sf::Vector2u(width, height)), "Galaga"); //I think this is how a window is setup
}

GameEngine::~GameEngine() {
    // Cleanup resources
    mWindow->close();

    //Kill objects
    for (const auto& obj : mGameObjects) {
        obj.reset(); //This is delete for shared_ptrs
    }
}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    mGameObjects.push_back(gameObject);
}

/**
 * @method Run
 * @arguments None
 * @description Gives control to the game engine. Will not return until the game window is closed or
 * all objects have been destroyed.
 */
void GameEngine::Run() {
    while (true)  // window is open
    {
        // 0. Remove any objects that are now dead
        RemoveDead();

        // 1. Activate and initialize any objects added during the last frame

        // 2. Process events

        // 3. Update game objects
        for (const auto& obj : mGameObjects) {
            obj->Update();
        }

        // 4. Process collision events

        // 5. Late updates
        for (const auto& obj : mGameObjects) {
            obj->LateUpdate();
        }

        // Clear window
        window.clear(CMPUT350::Colors::black);

        // 6. Render background

        // 7. Render foreground

        // Actually render to window
        
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
		}
		else if (event->is<sf::Event::Resized>())
		{
            // Not quite sure what goes here, DrawContexts seems to handle it
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
		{
			// use keyPressed->unicode to get character
            if (keyPressed->unicode == ' '){
                // I'm not quite sure what to call
            }
            if (keyPressed->unicode == 'a'){

            }
            if (keyPressed->unicode == 'd'){

            }
		}
	}
}


}  // namespace CMPUT350
