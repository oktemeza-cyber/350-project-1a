#ifndef ENEMY1_H
#define ENEMY1_H


#include "CollisionObject.h"
#include "GameContext.h"
#include <memory>
#include <vector>
#include "Bezier.h"
#include "Enemy.h"

class Enemy1 : public Enemy {
public:
	Enemy1(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path);
	void Initialize(CMPUT350::GameContext *context) override;
	void LateUpdate(CMPUT350::GameContext *context) override;
	bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) override;
	void RenderForeground(CMPUT350::GameContext *context) override;
};
#endif