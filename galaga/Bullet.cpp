#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player): isAlive(true), playerBullet(player), start(location), final(heading), bounds(location, 2){}

bool Bullet::IsPlayerBullet()
{
    return playerBullet;
}

void Bullet::Initialize(CMPUT350::GameContext* context){}
//no assets need initializing

void Bullet::Update(CMPUT350::GameContext* context)
{
        start.x += final.x * 10.0f;
        start.y += final.y * 10.0f;
        bounds = CMPUT350::Rect(start, 2.0f);


    if(start.y < 0){
        Kill();
    }
    
}

void Bullet::LateUpdate(CMPUT350::GameContext* context)
{
    //no delayed updates necessary
}

bool Bullet::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    //No key events for a bullet--The ship should spawn it in instead
    return false;
}

void Bullet::RenderBackground(CMPUT350::GameContext* context)
{
    //not necessary
}

void Bullet::RenderForeground(CMPUT350::GameContext* context)
{
    context->ScreenContext->DrawLine(start, start+5, 2.0, CMPUT350::Colors::red);
}

void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    Player* player = dynamic_cast<Player*>(obj.get());
        if(!playerBullet && player != nullptr){
            Kill();  
        }  
}
//idk if enemies shoot bullets in part a but here's partial implementation anyways

void Bullet::Kill()
{
    isAlive = false;
}

bool Bullet::IsAlive() const
{
    return isAlive;
}

const CMPUT350::Rect& Bullet::GetBounds()
{
    return bounds;
}
    
