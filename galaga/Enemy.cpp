#include <vector>
#include <cmath>
#include <cstdlib> //rand for attack testing
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"

int Enemy::arrivedEnemies = 0;

Enemy::Enemy(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path, int rando) : isAlive(true), sBounds(path.front(), 15.0f), health(1), attacking(false),
inFormation(false), formationLoc(loc), enemyLoc(path.front()), rotation(0.0f), progress(0.0f), progressMax(static_cast<float>((path.size() - 1) / 3)), bezier(path),
hasArrived(false), attackDelay(rando % 300), fireCooldown(0){ arrivedEnemies++, shapes.push_back(CMPUT350::Shape(CMPUT350::Rect(CMPUT350::Point2D(0, 0), 15.0f)));}


void Enemy::Update(CMPUT350::GameContext* context)
{
    if(!inFormation){
        CMPUT350::Point2D slope = bezier.GetSlope(progress);
        float len = std::sqrt(slope.x * slope.x + slope.y * slope.y);

        if(len > 0.000f){
            progress += 8.0/len;
            rotation = std::atan2(slope.y, slope.x);
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
        
        float frame = static_cast<float>(context->currentFrame);
        enemyLoc.x = formationLoc.x + 20.0f * std::sin(frame * 0.05f);
        enemyLoc.y = formationLoc.y;

        //temp
        if(!hasArrived){
            hasArrived = true;
            arrivedEnemies--;
        }

        if(arrivedEnemies == 0 && !attacking && !pauseAttack){
            attackDelay--;
            // if(attackDelay <= 0){
            //     attackDelay = 90 + rand()  % 300;
            //     //temp--this will just go to a specified corner rather than the player. Galaga will actually aim at the player
            //     CMPUT350::Point2D target(context->ScreenContext->GetWindowWidth() / 2.0f, 900.0f);
            //     Attack(context, target);
            // }
        }
    }

    if(attacking){
        fireCooldown--;
        if(fireCooldown <= 0){
            fireCooldown = 60;
            auto b = std::make_shared<Bullet>(enemyLoc, CMPUT350::Point2D(0,1), false, weak_from_this());
            context->mEngineView->AddGameObject(b);
        }
    }
    
    sBounds = CMPUT350::Rect(enemyLoc, 15.0f);
}

void Enemy::startPath(const std::vector<CMPUT350::Point2D> &controlPoints){
    bezier = CMPUT350::Bezier(controlPoints);
    progress = 0.0f;
    progressMax = static_cast<float>((controlPoints.size() - 1) / 3);
    inFormation = false;
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
    CollisionEnter(obj, CMPUT350::Point2D(0.0f, 0.0f));
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
    if(!hasArrived){
        hasArrived = true;
        arrivedEnemies--;
    }
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
    return enemyLoc; 
}

float Enemy::GetRotation() const {
    return rotation; 
}

const std::vector<CMPUT350::Shape>& Enemy::GetShapes() {
    return shapes; 
}

void Enemy::PauseAttacking(){
    pauseAttack = true;
}

void Enemy::UnpauseAttacking(){
    pauseAttack = false;
}
