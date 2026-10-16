#include <cassert>
#include <vector>
#include <memory>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

Player::Player(CMPUT350::Point2D loc): isAlive(true), playerLocation(loc), playerBounds(playerLocation, 20.0f), shapes{}
{}

void Player::setSelf(std::weak_ptr<CMPUT350::GameObject> selfPtr){
    self = selfPtr;
}
//helper to set parent

void Player::Initialize(CMPUT350::GameContext* context){
    context->mNotificationManager->Register(self, "MoveLeft");
    context->mNotificationManager->Register(self, "MoveRight");
    context->mNotificationManager->Register(self, "StopLeft");
    context->mNotificationManager->Register(self, "StopRight");
    context->mNotificationManager->Register(self, "Shoot");

    shapes.clear();

    shapes.emplace_back(CMPUT350::Rect(-5.0f, -20.0f, 10.0f, 30.0f));
    shapes.emplace_back(CMPUT350::Rect(-20.0f, -10.0f, 40.0f, 10.0f));
    shapes.emplace_back(CMPUT350::Rect(-13.0f, -5.0f, 25.0f, 10.0f));
    shapes.emplace_back(CMPUT350::Rect(-3.0f, -20.0f, 6.0f, 6.0f));
}
//no assets need initializing

void Player::Update(CMPUT350::GameContext* context)
{
    if(movingLeft){
        playerLocation.x -= 5.0f;
    }

    if(movingRight){
        playerLocation.x += 5.0f;
    }

    if(shooting){
        for(int i = 0; i < 2; i++){
            if(activeBullets[i].expired()){
                auto createdBullet = std::make_shared<Bullet>(playerLocation, CMPUT350::Point2D(0, -1), true, self);
                activeBullets[i] = createdBullet;
                context->mEngineView->AddGameObject(createdBullet);
                break;
            }
        }        
        shooting = false; // Reset shooting after shot (so doesn't double fire)
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

}//Handler for emptying the activeBullet array so that another bullet can be shot if a free space out of the 2 opens up

void Player::ReceiveNotification(const std::string &key)
{
    if(key == "MoveLeft"){
        movingLeft = true;
        movingRight = false;
    }
    else if(key == "MoveRight"){
        movingRight = true;
        movingLeft = false;
    }
    else if(key == "Shoot"){
        shooting = true;
    }
    else if(key == "StopLeft"){
        movingLeft = false;
    }
    else if(key == "StopRight"){
        movingRight = false;
    }
}

//on space press, create a new bullet using AddGameObject if any space in the array is a nullptr, then immediately break loop to not add more if two free spots are available.
//otherwise, don't add any more bullets than the lngth of the activeBullets (which is 2 in this case)

void Player::RenderBackground(CMPUT350::GameContext* context){}

void Player::RenderForeground(CMPUT350::GameContext* context)
{
    CMPUT350::Rect shipBody(-5.0f, -20.0f, 10.0f,30.0f);
    CMPUT350::Rect wings(-20.0f, -10.0f, 40.0f, 10.0f);
    CMPUT350::Rect wings2(-13.0f, -5.0f, 25.0f, 10.0f);
    CMPUT350::Rect cockpit(-3.0f, -20.0f, 6.0, 6.0f);

    
    context->ScreenContext->DrawRect(wings, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(wings2, CMPUT350::Colors::red);
    context->ScreenContext->DrawRect(shipBody, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(cockpit, CMPUT350::Colors::yellow);
    
}
//simply draws the ship as a collection of rects

void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject> &obj){
    CollisionEnter(obj, playerLocation);
}

void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>&obj, const CMPUT350::Point2D &collisionPoint)
{
     Bullet* bullet = dynamic_cast<Bullet*>(obj.get());
        if(bullet != nullptr && !bullet->IsPlayerBullet()){
            Kill();  
            bullet->Kill();
            return;
        }  

        if(dynamic_cast<Enemy *>(obj.get()) != nullptr){
            Kill();
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

CMPUT350::Point2D Player::GetLocation() const {
    return playerLocation;
}

float Player::GetRotation() const {
    return 0;
}

const std::vector<CMPUT350::Shape>& Player::GetShapes() {
    worldShapes.clear();

    for (const auto& shape : shapes) {
        switch (shape.t) {
            case CMPUT350::ShapeType::kLine: {
                CMPUT350::Line line = shape.shape.line;
                line.p1 += playerLocation;
                line.p2 += playerLocation;
                worldShapes.emplace_back(line);
                break;
            }

            case CMPUT350::ShapeType::kCircle: {
                CMPUT350::Circle circle = shape.shape.circle;
                circle.center += playerLocation;
                worldShapes.emplace_back(circle);
                break;
            }

            case CMPUT350::ShapeType::kRect: {
                CMPUT350::Rect rect = shape.shape.rect;
                rect += playerLocation;
                worldShapes.emplace_back(rect);
                break;
            }
        }
    }

    return worldShapes;
}