#include "GameEngine.h"
#include <memory>
#include "CollisionObject.h"
#include "GraphicsObject.h"
#include "DrawContext.h"
#include "GameContext.h"
#include "Keyboard.h"

/// @brief
namespace CMPUT350 {
#include "FontData.h"

//Big boy switch statement converts shapes into separate parts.
bool ShapesOverlap(const Shape& a, const Shape& b) {
    switch (a.t) {
        case ShapeType::kLine:
            switch (b.t) {
                case ShapeType::kLine:
                    return LineLine(a.shape.line, b.shape.line);

                case ShapeType::kCircle:
                    return LineCircle(
                        a.shape.line, b.shape.circle
                    );

                case ShapeType::kRect:
                    return LineRect(
                        a.shape.line, b.shape.rect
                    );
            }
            break;

        case ShapeType::kCircle:
            switch (b.t) {
                case ShapeType::kLine:
                    return LineCircle(
                        b.shape.line, a.shape.circle
                    );

                case ShapeType::kCircle:
                    return CircleCircle(
                        a.shape.circle, b.shape.circle
                    );

                case ShapeType::kRect:
                    return RectCircle(
                        b.shape.rect, a.shape.circle
                    );
            }
            break;

        case ShapeType::kRect:
            switch (b.t) {
                case ShapeType::kLine:
                    return LineRect(
                        b.shape.line, a.shape.rect
                    );

                case ShapeType::kCircle:
                    return RectCircle(
                        a.shape.rect, b.shape.circle
                    );

                case ShapeType::kRect:
                    return RectRect(
                        a.shape.rect, b.shape.rect
                    );
            }
            break;
    }

    return false;
}


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
    DrawContext screenContext(mWindow, mFont);
    DrawContext guiContext(mWindow, mFont);
    NotificationManager mNotif;

    GameContext mGContext;
    mGContext.mEngineView = this;
    mGContext.ScreenContext = &screenContext;
    mGContext.GUIContext = &guiContext;
    mGContext.currentFrame = 0;
    mGContext.mNotificationManager = &mNotif;


    while (mWindow->isOpen())  // window is open
    {
        mWindow->clear(sf::Color::Black); //moved here so that drawing from update was possible (for text)

        // 0. Remove any objects that are now dead
        RemoveDead();

        // 1. Activate and initialize any objects added during the last frame
        for (auto& newObj : mPending) {
            mGContext.CurrObject = newObj;
            newObj->Initialize(&mGContext);
            mGameObjects.push_back(newObj);
        }
        mPending.clear();

        // 2. Process events
        GameEngine::ProcessEvents(&mGContext);

        // 3. Update game objects
        for (auto& obj : mGameObjects) {
            mGContext.CurrObject = obj;
            obj->Update(&mGContext);
        }

        // 4. Process collision events
        for (size_t i = 0; i < mGameObjects.size(); i++){ //Straight from description
            std::shared_ptr<CollisionObject> objA = std::dynamic_pointer_cast<CollisionObject>(mGameObjects[i]);
            if (objA == nullptr) continue; // Not a collision object, skip
            for (size_t j = i+1; j < mGameObjects.size(); j++){
                std::shared_ptr<CollisionObject> objB = std::dynamic_pointer_cast<CollisionObject>(mGameObjects[j]);
                if (objB == nullptr) continue;
                
                const Rect &aBounds = objA->GetBounds();
                const Rect &bBounds = objB->GetBounds();

                if (RectIntersects(aBounds, bBounds)){
                    const auto& shapesA = objA->GetShapes();
                    const auto& shapesB = objB->GetShapes();
                    bool collided = false;

                    for (const Shape& a : shapesA) {
                        for (const Shape& b : shapesB) {
                            if (ShapesOverlap(a, b)) {
                                collided = true;
                                break;
                            }
                        }

                        if (collided) break;
                    }

                    if (collided) {
                        objA->CollisionEnter(objB);
                        objB->CollisionEnter(objA);
                    }
                }
            }
        }

        // 5. Late updates
        for (auto& obj : mGameObjects) {
            mGContext.CurrObject = obj;
            obj->LateUpdate(&mGContext);
        }

        // Clear window
        //mWindow->clear(sf::Color::Black); moved so galaga can draw text.

        // 6. Render background
        for (size_t i = 0; i < mGameObjects.size(); i++){
            std::shared_ptr<GraphicsObject> objA = std::dynamic_pointer_cast<GraphicsObject>(mGameObjects[i]);
            if (objA == nullptr) continue; // Not a graphics object, skip
            screenContext.SetContextOffset(objA->GetLocation());
            screenContext.SetContextRotation(objA->GetRotation());
            objA->RenderBackground(&mGContext);
        }

        // 7. Render foreground
        for (size_t i = 0; i < mGameObjects.size(); i++){
            std::shared_ptr<GraphicsObject> objA = std::dynamic_pointer_cast<GraphicsObject>(mGameObjects[i]);
            if (objA == nullptr) continue; // Not a graphics object, skip
            screenContext.SetContextOffset(objA->GetLocation());
            screenContext.SetContextRotation(objA->GetRotation());
            objA->RenderForeground(&mGContext);
        }

        // Actually render to window
        mWindow->display();

        if (mGameObjects.empty()) break;

        mGContext.currentFrame++;
    }
}

// Sample code for processing events

bool GameEngine::ProcessEvents(GameContext *context)
{
	while (const std::optional event = mWindow->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            mWindow->close();
            return true;
        }
        else if (event->is<sf::Event::Resized>())
        {
            return true;
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            int key = static_cast<int>(keyPressed->scancode);
            auto it = mKeyNotifsDown.find(key);
            if (it != mKeyNotifsDown.end()) {
                context->mNotificationManager->Notify(it->second);
            }
            return true;
        }
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            int key = static_cast<int>(keyReleased->scancode);
            auto it = mKeyNotifsUp.find(key);
            if (it != mKeyNotifsUp.end()) {
                context->mNotificationManager->Notify(it->second);
            }
            return true;
        }
    }
    return false;
}

void GameEngine::InstallKeyDownNotification(int key, const std::string &notification) {
    mKeyNotifsDown[key] = notification;
}

void GameEngine::InstallKeyUpNotification(int key, const std::string &notification) {
    mKeyNotifsUp[key] = notification;
}

EngineView::const_iterator GameEngine::cbegin() const {
    return mGameObjects.cbegin();
}

EngineView::const_iterator GameEngine::cend() const {
    return mGameObjects.cend();
}

EngineView::const_iterator GameEngine::begin() const {
    return mGameObjects.begin();
}

EngineView::const_iterator GameEngine::end() const {
    return mGameObjects.end();
}



}  // namespace CMPUT350
