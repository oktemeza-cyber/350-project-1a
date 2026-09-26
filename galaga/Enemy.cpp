#include "Enemy.h"
#include "Bullet.h"


Enemy::Enemy(CMPUT350::Point2D loc) : isAlive(true), enemyLoc(loc), sBounds(enemyLoc, 30){}

void Enemy::Initialize(CMPUT350::GameContext* context)
{
}

void Enemy::Update(CMPUT350::GameContext* context)
{
    //enemies don't move for project 1a, so maybe empty?
}

void Enemy::LateUpdate(CMPUT350::GameContext* context)
{
    //same reasoning as above
}

bool Enemy::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    return false;
    //enemies don't have key inputs, so empty
}

void Enemy::RenderBackground(CMPUT350::GameContext* context)
{
    //enemies don't have a bg
}


void Enemy::RenderForeground(CMPUT350::GameContext* context)
{
    context->ScreenContext->DrawRect(sBounds, CMPUT350::Colors::yellow);
}

void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    Bullet* bullet = dynamic_cast<Bullet*>(obj.get());
        if(bullet != nullptr && bullet->IsPlayerBullet()){
            Kill();  
            bullet->Kill();
        }  
}
//check if the bullet exists and the bullet is a player bullet, then kill the enemy and the bullet

void Enemy::Kill()
{
    isAlive = false;
}

bool Enemy::IsAlive() const
{
    return isAlive;
}

const CMPUT350::Rect& Enemy::GetBounds()
{
    // TODO: Update code
    return sBounds;
}
