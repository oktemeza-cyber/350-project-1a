#include "Enemy.h"
#include "Bullet.h"


Enemy::Enemy(CMPUT350::Point2D loc) : isAlive(true), enemyLoc(loc), sBounds(enemyLoc, 30)
{
    // TODO: Update code
    
}

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
    //left empty for bullet implementation, come back to finish
}

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
