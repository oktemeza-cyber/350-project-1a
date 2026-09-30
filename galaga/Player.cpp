#include <cassert>
#include "Player.h"
#include "Bullet.h"

Player::Player(CMPUT350::Point2D loc): isAlive(true), playerLocation(loc), playerBounds(playerLocation, 20.0f)
{
    activeBullets[0] = nullptr;
    activeBullets[1] = nullptr;
}

void Player::Initialize(CMPUT350::GameContext* context){}
//no assets need initializing

void Player::Update(CMPUT350::GameContext* context)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        playerLocation.x -= 5.0f;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        playerLocation.x += 5.0f;
    }
    //ended up putting the movement input here since update can actively check each frame if a or b is being pressed.
    //Handle input made it so that you can't stay still unless you press another button, continously moving left or right,
    //but also did not make it possible to shoot bullets while moving :(

    if(playerLocation.x < 0){
        playerLocation.x = 0;
    }

    if(playerLocation.x > context->ScreenContext->GetWindowWidth()){
        playerLocation.x = context->ScreenContext->GetWindowWidth();
    }

    playerBounds = CMPUT350::Rect(playerLocation, 20.0f);
    //update playerBounds and keep player within the window

}

void Player::LateUpdate(CMPUT350::GameContext* context)
{
    
    for(int i = 0; i < 2; i++){
        if(activeBullets[i] != nullptr && !activeBullets[i]->IsAlive()){
            activeBullets[i] = nullptr;
        }
    }

}//Handler for emptying the activeBullet array so that another bullet can be shot if a free space out of the 2 opens up

bool Player::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    if(key == ' '){
        for(int i = 0; i < 2; i++){
            if(activeBullets[i] == nullptr){
                Bullet* createdBullet = new Bullet(playerLocation, CMPUT350::Point2D(0, -1), true);
                activeBullets[i] = createdBullet;

                std::shared_ptr<CMPUT350::GameObject> newBullet(createdBullet);
                context->mEngineView->AddGameObject(newBullet);
                break;
            }
        }
        return true;
    }
    
    return false;
} 
//on space press, create a new bullet using AddGameObject if any space in the array is a nullptr, then immediately break loop to not add more if two free spots are available.
//otherwise, don't add any more bullets than the lngth of the activeBullets (which is 2 in this case)

void Player::RenderBackground(CMPUT350::GameContext* context){}

void Player::RenderForeground(CMPUT350::GameContext* context)
{
    CMPUT350::Rect shipBody(playerLocation.x - 5.0f, playerLocation.y - 20.0f, 10.0f,30.0f);
    CMPUT350::Rect wings(playerLocation.x - 20.0f, playerLocation.y - 10.0f, 40.0f, 10.0f);
    CMPUT350::Rect wings2(playerLocation.x - 13.0f, playerLocation.y - 5.0f, 25.0f, 10.0f);
    CMPUT350::Rect cockpit(playerLocation.x - 3.0f, playerLocation.y - 20.0f, 6.0, 6.0f);

    
    context->ScreenContext->DrawRect(wings, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(wings2, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(shipBody, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(cockpit, CMPUT350::Colors::yellow);
}
//simply draws the ship as a collection of rects

void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
     Bullet* bullet = dynamic_cast<Bullet*>(obj.get());
        if(bullet != nullptr && !bullet->IsPlayerBullet()){
            Kill();  
            bullet->Kill();
        }  
}
//checks to see if the bullet exists and IsPlayerBullet is false, but playerBullet is automatically true in player.cpp on creation, so the bullet SHOULD never delete
//when its first created

void Player::Kill()
{
    isAlive = false;
}

bool Player::IsAlive() const
{
    return isAlive;
}

const CMPUT350::Rect& Player::GetBounds()
{
    return playerBounds;
}
