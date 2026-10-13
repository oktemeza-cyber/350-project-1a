#ifndef ENEMY_H
#define ENEMY_H


#include "CollisionObject.h"
#include "GameContext.h"
#include <memory>
#include <vector>
#include "Bezier.h"

class Enemy : public CMPUT350::CollisionObject, public std::enable_shared_from_this<Enemy> //REMOVE enable_shared_from_this LATER, galaga.cpp will handle attacking but this is for testing purposes (reference here: https://en.cppreference.com/cpp/memory/enable_shared_from_this)
{
public:
    Enemy(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path);
    virtual bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) = 0;

    // GameObject Functions
    void Update(CMPUT350::GameContext* context) override;
    void LateUpdate(CMPUT350::GameContext* context) override;
    bool IsAlive() const override;
    void Kill() override;

    // Graphics Object Functions
    void RenderForeground(CMPUT350::GameContext* context) override;
    CMPUT350::Point2D GetLocation() const override;
    float GetRotation() const override;

    // Collision Object Functions
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) override;
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject> &obj, const CMPUT350::Point2D &collisionPoint) override;
    const CMPUT350::Rect& GetBounds() override;
    const std::vector<CMPUT350::Shape>& GetShapes() override;

protected:
    void startPath(const std::vector<CMPUT350::Point2D> &controlPoints);
    int health;
    bool attacking;
    bool inFormation;
    CMPUT350::Point2D formationLoc;
    CMPUT350::Point2D enemyLoc;
  //protected variables so that other enemy types can use this but main can't access these
    
private:
    bool isAlive;
    CMPUT350::Rect sBounds;
    float rotation;
    float progress;
    float progressMax;
    CMPUT350::Bezier bezier;  
    std::vector<CMPUT350::Shape> shapes;

    //possible temp attack variable start
    static int arrivedEnemies;
    bool hasArrived;
    int attackDelay;
    int fireCooldown;
};


#endif

