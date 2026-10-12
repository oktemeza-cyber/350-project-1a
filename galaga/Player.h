#ifndef PLAYER_H
#define PLAYER_H

#include "CollisionObject.h"

class Bullet;

class Player : public CMPUT350::CollisionObject
{
public:
    Player(CMPUT350::Point2D loc);
    void setSelf(std::weak_ptr<CMPUT350::GameObject> selfPtr); // this is for the parent variable in relation to bullet

    // GameObject Functions
    void Initialize(CMPUT350::GameContext* context) override;
    void Update(CMPUT350::GameContext* context) override;
    void LateUpdate(CMPUT350::GameContext* context) override;
    bool IsAlive() const override;
    void Kill() override;

    void ReceiveNotification(const std::string &key) override;

    // Graphics Object Functions
    void RenderBackground(CMPUT350::GameContext* context) override;
    void RenderForeground(CMPUT350::GameContext* context) override;
    CMPUT350::Point2D GetLocation() const override;
    float GetRotation() const override;

    // Collision Object Functions
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) override;
    const CMPUT350::Rect& GetBounds() override;
    const std::vector<CMPUT350::Shape>& GetShapes() override;

private:
    bool isAlive;
    CMPUT350::Point2D playerLocation;
    CMPUT350::Rect playerBounds;
    std::weak_ptr<Bullet> activeBullets[2];
    std::vector<CMPUT350::Shape> shapes;
    std::weak_ptr<CMPUT350::GameObject> self; //parent continued 

    bool movingLeft = false;
    bool movingRight = false;
    bool shooting = false;
};

#endif
