#include <vector>
#include <cmath>
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"


Enemy::Enemy(CMPUT350::Point2D loc, std::vector<CMPUT350::Point2D> &path) : isAlive(true), sBounds(path.front(), 15.0f), shapes{}, health(1), attacking(false),
inFormation(false), formationLoc(loc), enemyLoc(path.front()), rotation(0.0f), progress(0.0f), progressMax(static_cast<float>((path.size() - 1) / 3)), bezier(path){}


void Enemy::Update(CMPUT350::GameContext* context)
{
    if(!inFormation){
        CMPUT350::Point2D slope = bezier.GetSlope(progress);
        float len = std::sqrt(slope.x * slope.x + slope.y * slope.y);

        if(len > 0.000f){
            progress += 8.0/len;
            rotation = std::atan2(slope.y, slope.x) * 180.0f / 3.14159265f;
        }

        if(progress >= progressMax){
            progress = progressMax;
            inFormation = true;
            attacking = false;
            rotation = 0.0f;
        }
        enemyLoc = bezier.GetPoint(progress);
    }
    else{
        enemyLoc = formationLoc;
    }
    sBounds = CMPUT350::Rect(enemyLoc, 15.0f);
}

void Enemy::LateUpdate(CMPUT350::GameContext* context)
{
    //same reasoning as above
}


void Enemy::RenderForeground(CMPUT350::GameContext* context)
{
    context->ScreenContext->DrawRect(sBounds, CMPUT350::Colors::yellow);
}

void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj){
    CollisionEnter(obj, enemyLoc);
}

void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj, const CMPUT350::Point2D &collisionPoint)
{
    Bullet* bullet = dynamic_cast<Bullet*>(obj.get());
        if(bullet != nullptr && bullet->IsPlayerBullet()){
            health--;
            if(health <= 0){
                Kill();
            }
            return; 
        }

        if(dynamic_cast<Player *>(obj.get()) != nullptr){
            Kill();
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
    return sBounds;
}

CMPUT350::Point2D Enemy::GetLocation() const {
    return enemyLoc; // todo
}

float Enemy::GetRotation() const {
    return rotation; // todo
}

const std::vector<CMPUT350::Shape>& Enemy::GetShapes() {
    return shapes; // todo
}
