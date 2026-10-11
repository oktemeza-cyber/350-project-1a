#ifndef BULLET_H
#define BULLET_H

#include "CollisionObject.h"
#include "GameContext.h"

class Bullet : public CMPUT350::CollisionObject
{
public:
    public:
	Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player, std::weak_ptr<CMPUT350::GameObject> parent);
	bool IsPlayerBullet();
	void Update(CMPUT350::GameContext *context);
	void LateUpdate(CMPUT350::GameContext *context);
	void RenderForeground(CMPUT350::GameContext *context);
	bool IsAlive() const override;
	void Kill() override;
	const std::vector<CMPUT350::Shape> &GetShapes() override;
	void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject> &obj) override;
	void CollisionEnter(const std::shared_ptr<CollisionObject> &obj, const CMPUT350::Point2D &collisionPoint) override;
	const CMPUT350::Rect &GetBounds() override;
	
	CMPUT350::Point2D GetLocation() const override;
	float GetRotation() const override;

//all copy pasted from the document in the project 1b specifications

private:
    bool isAlive;
    bool playerBullet;
    CMPUT350::Point2D start;
    CMPUT350::Point2D previous;
    CMPUT350::Point2D final;
    CMPUT350::Rect bounds;
    std::vector<CMPUT350::Shape> shapes; //maybe temp fix? idk - it was the fix horray

    std::weak_ptr<CMPUT350::GameObject> parent;
    bool hasHit;
    CMPUT350::Point2D hitPoint;
    
};
#endif // BULLET_H
