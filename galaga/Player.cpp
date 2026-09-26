#include <cassert>
#include "Player.h"
#include "Bullet.h"

Player::Player(CMPUT350::Point2D loc): isAlive(true), playerLocation(loc), playerBounds(playerLocation, 20), movingLeft(false), movingRight(false)
{
    activeBullets[0] = nullptr;
    activeBullets[1] = nullptr;
}

void Player::Initialize(CMPUT350::GameContext* context){}

void Player::Update(CMPUT350::GameContext* context)
{
    if(movingLeft){
        playerLocation.x -= 5.0;
    }

    if(movingRight){
        playerLocation += 5.0;
    }

    if(playerLocation.x < 0){
        playerLocation.x = 0;
    }

    if(playerLocation.x > context->ScreenContext->GetWindowWidth()){
        playerLocation.x = context->ScreenContext->GetWindowWidth();
    }

    playerBounds = CMPUT350::Rect(playerLocation, 20);


}

void Player::LateUpdate(CMPUT350::GameContext* context)
{
    
    for(int i = 0; i < 2; i++){
        if(activeBullets[i] != nullptr && activeBullets[i]->IsAlive()){
            delete activeBullets[i];
            activeBullets[i] = nullptr;
        }
    }

}

bool Player::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    if(key == 'a'){
        movingLeft = true;
        return true;
    } else{
        movingLeft = false;
        return true;
    }
    
    if(key == 'd'){
        movingRight = true;
        return true;
    } else{
        movingRight = false;
        return true;
    }

    
    if(key == ' '){
        for(int i = 0; i < 2; i++){
            if(activeBullets[i] == nullptr){
                Bullet* createdBullet = new Bullet(playerLocation, CMPUT350::Point2D(0, -1), true);
                activeBullets[i] = createdBullet;
                //context->mEngineView->AddGameObject(createdBullet); wait till this is implemented
                break;
            }
        }
    }
    
    return false;
}

void Player::RenderBackground(CMPUT350::GameContext* context)
{
}

void Player::RenderForeground(CMPUT350::GameContext* context)
{
    //not implemented yet until game engine functions to check Rect shapes and testing
}

void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
     Bullet* bullet = dynamic_cast<Bullet*>(obj.get());
        if(bullet != nullptr && !bullet->IsPlayerBullet()){
            Kill();  
            bullet->Kill();
        }  
} //idk if enemies shoot bullets in part a but here's the set-up anyways

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
