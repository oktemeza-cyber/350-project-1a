#include <vector> 
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

Bullet::Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player, std::weak_ptr<CMPUT350::GameObject> parent): isAlive(true), playerBullet(player), 
start(location), final(heading), previous(location), bounds(location, location), parent(parent), hasHit(false), hitPoint(location){}

bool Bullet::IsPlayerBullet()
{
    return playerBullet;
}

void Bullet::Update(CMPUT350::GameContext* context)
{

    std::shared_ptr<CMPUT350::GameObject> bulletParent = parent.lock();
    if(bulletParent == nullptr || !bulletParent->IsAlive()){
        Kill();
        return;
    }
    
    previous = start;
    start.x += final.x * 10.0f;
    start.y += final.y * 10.0f;

    shapes.clear();
    shapes.push_back(CMPUT350::Shape(CMPUT350::Line(previous, start)));


    bounds = CMPUT350::Rect(previous, start);
    bounds.topLeft.x -= 1.0f;
    bounds.width += 2.0f;


    if(start.y < 0 || start.y > context->ScreenContext->GetWindowHeight()){
        Kill();
    }
    
   
}

void Bullet::LateUpdate(CMPUT350::GameContext* context)
{
    //no delayed updates necessary
}




void Bullet::RenderForeground(CMPUT350::GameContext* context)
{
    if (hasHit){
        context->ScreenContext->DrawCircle(hitPoint, 5.0f, CMPUT350::Colors::green);
    } else if(isAlive){

        if(playerBullet){
            context->ScreenContext->DrawLine(previous, start, 2.0f, CMPUT350::RGBColor(255, 20, 20));
        } else {
            context->ScreenContext->DrawLine(previous, start, 2.0f, CMPUT350::RGBColor(255, 0, 255));
            //This is for the enemy but idk how different the colour will be--make sure to test later
        }
    }
    
}

void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    CollisionEnter(obj, CMPUT350::Point2D(0.0f, 0.0f));
}

void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj, const CMPUT350::Point2D& collisionPoint){
    if(hasHit){
        return;
    }

    if(obj.get() == parent.lock().get()){
        return;
    }
    //ignores the ship for collision 

    if(playerBullet){
        if (dynamic_cast<Enemy *>(obj.get()) == nullptr){
            return;
        }
    } else {
        if (dynamic_cast<Player *>(obj.get()) == nullptr){
            return;
        }
    }

    hitPoint = start + collisionPoint;
    hasHit = true;
    Kill();
     //kill no matter what after collision
}


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

CMPUT350::Point2D Bullet::GetLocation() const {
    return CMPUT350::Point2D(0,0); // todo
}

float Bullet::GetRotation() const {
    return 0.0f; 
}

const std::vector<CMPUT350::Shape>& Bullet::GetShapes() {
    return shapes; 
}

    
